#include "app/game/block.hpp"
#include "app/game/controller.hpp"
#include "app/game/entities/entity.hpp"
#include "app/game/enviroment_controller.hpp"
#include "app/game/particles/particle_block.hpp"
#include "app/game/particles/particle_bubble.hpp"
#include "app/game/tools/resource_controller.hpp"
#include "pge/input.hpp"
#include "pge/pge.hpp"
#include "player.hpp"

void Player::_jump(float hs)
{
    hs = hs * _getModJump(0, 1);
    v_speed = hs * (1 - abs(h_speed * (0.02 * _getModJumpSpd(0, 1))));
}

bool Player::_fall(float gravity, float flyspeed)
{
    float _center;
    y += v_speed;
    if (v_speed >= 0 && (control->block_move(&x, &y)
                            || (!_isFree(bt_stair2, 0, 0))
                            || (_isType(bt_platfm, 0, 1) && !(m_down && m_sprint) && y < (_center = int(y / 12) * 12) + 6))) {
        if (v_speed > 4.3) {
            life -= (v_speed - 4.3) * 38 * _getModFalldmg(0, 3);
        } else if (v_speed > 1)
            pge::chunk::play(chn_land[pge::random::get(0, 2)]);

        if (v_speed > 2)
            for (int i = pge::random::get(1, 2 + v_speed); i > 0; i--)
                control->create_particles(new Particle_Block(control, x + pge::random::get(-6, 6), y + pge::random::get(-2 - v_speed / 2, 0), 0, 0, *control->env()->bc_type(x / 12, (y + 4) / 12)));

        if (_isType(bt_platfm) && !(m_down && m_sprint)) {
            if (h_speed != 0 && y > _center + 0.1)
                y -= 0.05;
        }

        if (v_speed > 1 && !m_sprint) {
            v_speed = -v_speed * _getModBounce(0, 1);
            return true;
        } else {
            v_speed = 0;
            return false;
        }
    } else if (v_speed < 0 && !_isFree(bt_stair2, 0, -17)) {
        v_speed = 0;
        return false;
    } else if (v_speed < flyspeed) {
        if (m_jump && v_speed < 0)
            v_speed += 0.04 * gravity;
        else
            v_speed += 0.08 * gravity;
    } else if (v_speed > flyspeed)
        v_speed -= 0.01;
    return true;
}

void Player::_wallJump(float hs, float vs)
{
    v_speed = -vs;
    energy -= 4;
    pge::chunk::play(chn_land[pge::random::get(0, 2)]);

    if (control->env()->bc_check((x - 3) / 12, (y - 3) / 12) == bt_normal)
        h_speed = hs;
    else if (control->env()->bc_check((x + 3) / 12, (y - 3) / 12) == bt_normal)
        h_speed = -hs;

    for (int i = pge::random::get(3, 5); i > 0; i--)
        control->create_particles(new Particle_Block(control, x + (h_speed > 0 ? pge::random::get(0, 3) : -pge::random::get(0, 3)), y + pge::random::get(-4, 4), 0, 0, *control->env()->bc_type((x - h_speed * 4) / 12, (y - 3) / 12)));
}
bool Player::_wallFall()
{
    bool f = false;
    static bool falling = false;
    if (v_speed > 0
        && ((m_right && (f = _isType(bt_normal, 4, -2))) || (m_left && _isType(bt_normal, -4, -2)))) {
        if (f) {
            if (pge::random::get(0, 100) > 95)
                control->create_particles(new Particle_Block(control, x + pge::random::get(-4, 0), y + pge::random::get(-4, 4), 0, 0, *control->env()->bc_type((x + 6) / 12, (y - 3) / 12)));
            direction = SDL_FLIP_HORIZONTAL;
        } else {
            if (pge::random::get(0, 100) > 95)
                control->create_particles(new Particle_Block(control, x + pge::random::get(0, 4), y + pge::random::get(-4, 4), 0, 0, *control->env()->bc_type((x - 6) / 12, (y - 3) / 12)));
            direction = SDL_FLIP_NONE;
        }

        if (!tired && v_speed > 0.5) {
            v_speed -= 0.12;
            energy -= 0.16;
        } else {
            v_speed -= m_jump ? 0.02 : 0.06;
            energy -= 0.04;
        }

        return true;
    } else {
        falling = false;
        return false;
    }
}

void Player::_move(float mspd)
{
    mspd = mspd * _getModSpeed(0, 1);
    float spdup = 0.04 * _getModSpdUp(0, 1);
    float spdwn = 0.025 * _getModSpdSlow(0, 1);

    if (m_sprint)
        mspd = mspd * 0.5;
    else if (tired)
        mspd = mspd = 0.7;

    if (abs(h_speed) - spdwn <= spdwn / 2)
        h_speed = 0;
    else
        h_speed += h_speed > 0 ? (-spdwn) : (spdwn);

    if (m_right)
        if (h_speed + spdup <= mspd)
            h_speed += spdup;
    if (m_left)
        if (h_speed - spdup >= -mspd)
            h_speed -= spdup;

    if ((h_speed > 0 && _isFree(bt_stair2, 3, -2) && _isFree(bt_stair2, 3, -15)) || (h_speed < 0 && _isFree(bt_stair2, -3, -2) && _isFree(bt_stair2, -3, -15)))
        x += h_speed;
    else
        h_speed = 0;
}

bool Player::_climbLadder()
{
    if (climbing || (!m_sprint && m_up && _isType(bt_ladder, 0, -6)) || (!m_sprint && m_down && _isFree(bt_stair2, 0, 0) && _isType(bt_ladder, 0, -6))) {
        climbing = true;
        const float spdup = 0.02;
        const float spdwn = 0.01;
        const float mspd = 0.5;

        if (m_right)
            h_speed += spdup;
        if (m_left)
            h_speed -= spdup;
        if (m_down)
            v_speed += spdup;
        if (m_up)
            v_speed -= spdup;

        if (abs(h_speed) - spdwn <= spdwn / 2)
            h_speed = 0;
        else
            h_speed += h_speed > 0 ? (-spdwn) : (spdwn);

        if (abs(v_speed) - spdwn <= spdwn / 2)
            v_speed = 0;
        else
            v_speed += v_speed > 0 ? (-spdwn) : (spdwn);

        if (h_speed > 0 && (!_isFree(bt_stair2, 4, -2) || !_isFree(bt_stair2, 4, -16)))
            h_speed = 0;
        else if (h_speed < 0 && (!_isFree(bt_stair2, -4, -2) || !_isFree(bt_stair2, -4, -16)))
            h_speed = 0;

        if (v_speed > 0 && (!_isFree(bt_stair2, 0, 1) || _isType(bt_platfm, 0, 0))) {
            v_speed = 0;
            climbing = false;
            jumped = true;
            return false;
        } else if (v_speed < 0 && (!_isFree(bt_stair2, -3, -16) || !_isFree(bt_stair2, 3, -16)))
            v_speed = 0;

        if (v_speed > 0.5)
            v_speed = 0.5;
        if (v_speed < -0.5)
            v_speed = -0.5;
        if (h_speed > 0.5)
            h_speed = 0.5;
        if (h_speed < -0.5)
            h_speed = -0.5;

        float _center = int(x / 12) * 12 + 6;
        if (x > _center + 0.5)
            x -= 0.1;
        else if (x < _center - 0.5)
            x += 0.1;

        if (m_sprint)
            climbing = false;

        x += h_speed;
        y += v_speed;

        if (!_isType(bt_ladder, 0, -4)) {
            climbing = false;
            jumped = true;
            if (m_up)
                v_speed = -1.1;
            return false;
        }
        return true;
    }
    return false;
}

bool Player::_swim()
{
    static uint32_t breathing = 0;

    if (_isType(bt_water, 0, -6)) {
        if (!swimming) {
            for (int i = pge::random::get(3, 6); i > 0; i--) {
                float pos = float(pge::random::get(-60, 60)) / 10;
                control->create_particles(new Particle_Bubble(control, x + pos, y / 12 * 12 + pge::random::get(0, 2), pos * 0.05, 0.3 - abs(float(pos)) * 0.05));
                control->create_particles(new Particle_Block(control, x + pos, y / 12 * 12 - pge::random::get(0, 4), pos * 0.05, -v_speed / 3, aBlocks[b_water]));
            }
            v_speed = v_speed / 4;
            h_speed = h_speed / 2;
            swimming = true;
        }
        if (breathing < pge::get_time_passed()) {
            control->create_particles(new Particle_Bubble(control, x + pge::random::get(-1, 1), y - pge::random::get(9, 11), 0, 0.1));
            breathing = pge::get_time_passed() + pge::random::get(300, 700);
        }

        if (_isType(bt_water, 0, 1)) {
            _move(0);
            if (anim_active->get_frame() == 5) {
                if (m_left)
                    h_speed = -0.4;
                if (m_right)
                    h_speed = 0.4;

                if ((m_up || m_jump) && _isFree(bt_stair2, 0, -18) && v_speed > -0.5)
                    v_speed = -0.3;
            }
        } else {
            if (m_up || m_jump) {
                _jump(-0.5);
            }
            _move(0.33);
        }
        _fall(0.1, 0.2);
        return true;
    } else {
        if (swimming) {
            for (int i = pge::random::get(6, 10); i > 0; i--) {
                float pos = float(pge::random::get(-60, 60)) / 10;
                control->create_particles(new Particle_Bubble(control, x + pos, y / 12 * 12 + pge::random::get(0, 2), pos * 0.05, -abs(float(pos)) * 0.1));
            }
            swimming = false;
            if (m_up || m_jump)
                _jump(-1.2);
        }
        return false;
    }
}
