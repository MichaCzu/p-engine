#include "player.hpp"
#include "app/game/block.hpp"
#include "app/game/controller.hpp"
#include "app/game/entities/entity.hpp"
#include "app/game/enviroment_controller.hpp"
#include "app/game/particles/particle_block.hpp"
#include "app/game/tools/resource_controller.hpp"
#include "pge/input.hpp"
#include "pge/pge.hpp"

Player::Player(Controller* _source, float _x, float _y, uint16_t _life, PController* _pcontrol)
    : Entity(_source, _x, _y, _life)
{
    pcontrol = _pcontrol;
    sprite = control->res()->get_image(100, "res/spr/player/player.png");
    chk_damage[0] = control->res()->get_sound(120, "res/sfx/damage.wav");
    chk_damage[1] = control->res()->get_sound(121, "res/sfx/damage2.wav");
    chk_damage[2] = control->res()->get_sound(122, "res/sfx/damage3.wav");
    chk_damage[3] = control->res()->get_sound(123, "res/sfx/damage4.wav");
    chn_land[0] = control->res()->get_sound(105, "res/sfx/land1.wav");
    chn_land[1] = control->res()->get_sound(106, "res/sfx/land2.wav");
    chn_land[2] = control->res()->get_sound(107, "res/sfx/land3.wav");
    chn_step1 = control->res()->get_sound(108, "res/sfx/step1.wav");
    chn_step2 = control->res()->get_sound(109, "res/sfx/step2.wav");
    chn_tire1 = control->res()->get_sound(110, "res/sfx/tire1.wav");
    chn_tire2 = control->res()->get_sound(111, "res/sfx/tire2.wav");
    chn_ladder1 = control->res()->get_sound(112, "res/sfx/ladder1.wav");
    chn_ladder2 = control->res()->get_sound(113, "res/sfx/ladder2.wav");
}

Player::~Player() {}

void Player::prelogic(uint16_t diff) {}
void Player::collision(Entity* target) {}

void Player::fixedstep()
{
    if (!_climbLadder() && !_swim()) {
        _move(1);

        if (!_fall()) {
            if (m_jump && !jumped) {
                if (!tired)
                    _jump(-1.7);
                else
                    _jump(-1.4);
                jumped = true;
            } else if (!m_jump)
                jumped = false;
        }

        if (!m_sprint && _wallFall()) {
            if (m_jump && !jumped && !tired) {
                _wallJump(1.75, 1.4);
                jumped = true;
            } else if (!m_jump)
                jumped = false;
        }
    }

    //cheat
    if (m_hpdrain) {
        life -= 0.1;
    }
    if (m_hpup) {
        life += 0.1;
    }

    //energy
    static int drain_energy = 500, prev_energy = 0;
    if (abs(h_speed) > 0.5) {
        energy -= 0.005;
        drain_energy = pge::get_time_passed() + 200;
    } else if (prev_energy > energy)
        drain_energy = pge::get_time_passed() + 2000;

    if (energy <= 0) {
        if (!tired)
            pge::chunk::play(chn_tire1);
        tired = true;
        energy = 0;
    }

    if (tired && energy > 7 * 6) {
        pge::chunk::play(chn_tire2);
        tired = false;
    }

    if (energy < 7 * 8 && drain_energy < pge::get_time_passed())
        energy += tired ? 0.04 : 0.08;
    prev_energy = energy;

    //health
    static int taken_damage = 0, prev_life = 500;

    if (life > 7 * 8)
        life = 7 * 8;

    if (prev_life > life) {
        taken_damage = pge::get_time_passed() + 5000;
        pge::chunk::play(chk_damage[pge::random::get(0, 3)]);
    }

    if (!tired && int(life) % 7 > 0 && taken_damage < pge::get_time_passed()) {
        life += 0.08;
    }
    prev_life = life;

    if (energy > maxhealth)
        energy = maxhealth;

    if (life <= 0) {
        life = 1;
    }
}

void Player::logic(uint16_t diff)
{
    static bool a = 0;
    static bool b = 0;
    if (anim_active == &a_jump) {
        a = 0;
        b = 0;
    }

    if (!m_sprint) {
        if (h_speed < 0)
            direction = SDL_FLIP_HORIZONTAL;
        if (h_speed > 0)
            direction = SDL_FLIP_NONE;
    }

    if (anim_active == &a_run || anim_active == &a_walk) {
        if (a == 1 && anim_active->get_frame() == 0) {
            a = 0;
            pge::chunk::play(chn_step1, 0, -1, -1, abs(h_speed) * 100);

            if (anim_active == &a_run)
                for (int i = pge::random::get(3, 5); i > 0; i--)
                    control->create_particles(new Particle_Block(control, x + pge::random::get(-4, 4), y + pge::random::get(-4, 0), 0, 0, *control->env()->bc_type(x / 12, (y + 1) / 12), 75));
        } else if (anim_active->get_frame() != 0)
            a = 1;

        if (b == 1 && anim_active->get_frame() == 2) {
            b = 0;
            pge::chunk::play(chn_step2, 0, -1, -1, abs(h_speed) * 100);

            if (anim_active == &a_run)
                for (int i = pge::random::get(3, 5); i > 0; i--)
                    control->create_particles(new Particle_Block(control, x + pge::random::get(-4, 4), y + pge::random::get(-4, 0), 0, 0, *control->env()->bc_type(x / 12, (y + 1) / 12), 75));
        } else if (anim_active->get_frame() != 2)
            b = 1;

    } else if (anim_active == &a_ladder) {
        if (a && anim_active->get_frame() == 0) {
            a = 0;
            pge::chunk::play(chn_ladder1);
        } else if (anim_active->get_frame() != 0)
            a = 1;

        if (b && anim_active->get_frame() == 2) {
            pge::chunk::play(chn_ladder2);
            b = 0;
        } else if (anim_active->get_frame() != 2)
            b = 1;
    } else if (anim_active == &a_ladderhor) {
        if (h_speed < 0)
            direction = SDL_FLIP_HORIZONTAL;
        if (h_speed > 0)
            direction = SDL_FLIP_NONE;

        if (a && anim_active->get_frame() == 2) {
            b = b == 1 ? 0 : 1;
            if (b)
                pge::chunk::play(chn_ladder2);
            else
                pge::chunk::play(chn_ladder1);
            a = 0;
        } else if (anim_active->get_frame() != 2)
            a = 1;
    }

    if (swimming && _isType(bt_water, 0, 1)) {
        if ((m_up || m_left || m_right)) {
            if (anim_active != &a_swim)
                anim_active = a_swim.set(0);
            anim_active->animate(diff);
        } else if (anim_active->get_frame() != 0 && anim_active == &a_swim)
            anim_active->animate(diff / 2);
        else
            anim_active = a_jump.set(2);
    } else if (climbing) {
        if (v_speed == 0 && h_speed != 0 && anim_active != &a_ladderhor) {
            anim_active = a_ladderhor.set(0);
        } else if (v_speed != 0 && anim_active != &a_ladder)
            anim_active = a_ladder.set(0);

        if (v_speed != 0)
            anim_active->animate(diff * (abs(v_speed) * 6 + 1));
        else if (h_speed != 0)
            anim_active->animate(diff * (abs(h_speed) * 2 + 1));

    } else {
        if (h_speed == 0 || (!m_left && !m_right && _isType(bt_transp, 0, 1))) {
            if (anim_active != &a_idle) {
                anim_active = a_idle.set(anim_active->get_frame());
            }
        } else if (abs(h_speed) <= 0.6) {
            if (anim_active != &a_walk) {
                anim_active = a_walk.set(anim_active->get_frame());
            }
        } else {
            if (anim_active != &a_run) {
                anim_active = a_run.set(anim_active->get_frame());
            }
        }

        if (anim_active == &a_run || anim_active == &a_walk)
            diff = diff * (abs(h_speed * 2) + 1);

        if ((v_speed != 0 && direction == SDL_FLIP_HORIZONTAL && h_speed > 0) || (v_speed != 0 && direction == SDL_FLIP_NONE && h_speed < 0)) {
            if (v_speed < -0.4)
                anim_active = a_jump.set(2);
            else
                anim_active = a_jump.set(1);
        } else if (v_speed != 0 && v_speed > -0.6 && v_speed < 0.2 && control->env()->bc_check((x) / 12, (y + 1) / 12) == bt_empty) {
            anim_active = a_jump.set(1);
        } else if (v_speed < 0) {
            anim_active = a_jump.set(0);
        } else if (v_speed > 0) {
            anim_active = a_jump.set(2);
        } else {
            if (m_sprint && (direction == SDL_FLIP_HORIZONTAL ? h_speed > 0 : h_speed < 0))
                anim_active->animate(-diff);
            else
                anim_active->animate(diff);
        }
    }
}

void Player::draw(pge::viewport::View& vp)
{
    float s = pge::draw::get_scale();
    pge::Pixel center(6.5, 11);
    pge::image::draw(sprite, (x - vp.x + anim_active->get()->p.x) * s, (y - vp.y + anim_active->get()->p.y) * s, &anim_active->get()->r, &center, v_speed == 0 ? 0 : v_speed * 3 * h_speed, s, s, direction);
}

void Player::push(float _vx, float _vy) {}
void Player::take_damage(uint16_t value, Entity* source) {}
