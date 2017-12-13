#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "app/game/entities/entity.hpp"
//#include "app/game/player_controller.hpp"
#include "pge/pge.hpp"

class PController;

class Player : public Entity {
    //control flag
    static const uint16_t maxhealth = 7 * 8;
    //
    PController* pcontrol;
    SDL_RendererFlip direction = SDL_FLIP_NONE;
    uint16_t sprite;
    float energy = 33;
    bool tired = false;
    float v_speed = 0, h_speed = 0;
    bool climbing = false, swimming = false;
    bool jumped = false;

    void _jump(float hs);
    bool _fall(float gravity = 1, float flyspeed = 6);
    bool _wallFall();
    void _wallJump(float hs, float vs);
    void _move(float hs);
    bool _climbLadder();
    bool _swim();

    uint16_t chk_damage[4],
        chn_jump, chn_land[3], chn_step1, chn_step2, chn_tire1, chn_tire2, chn_ladder1, chn_ladder2;

    //Animations;
    Animation a_idle = Animation(2, std::vector<pge::Clip>{
                                        pge::Clip(-7, -18, 0, 0, 13, 18),
                                        pge::Clip(-7, -18, 0, 0, 13, 18),
                                        pge::Clip(-7, -18, 0, 0, 13, 18),
                                        pge::Clip(-7, -18, 0, 0, 13, 18),
                                        pge::Clip(-7, -18, 0, 0, 13, 18),
                                        pge::Clip(-7, -17.7, 13, 0, 13, 18),
                                        pge::Clip(-7, -17.7, 0, 0, 13, 18),
                                        pge::Clip(-7, -17.7, 0, 0, 13, 18) });
    Animation a_walk = Animation(2, std::vector<pge::Clip>{
                                        pge::Clip(-7, -18, 26, 0, 13, 18),
                                        pge::Clip(-7, -18.33, 39, 0, 13, 18),
                                        pge::Clip(-7, -18, 52, 0, 13, 18),
                                        pge::Clip(-7, -18.33, 65, 0, 13, 18) });
    Animation a_run = Animation(2, std::vector<pge::Clip>{
                                       pge::Clip(-7, -18, 26, 18, 13, 18),
                                       pge::Clip(-7, -18.66, 39, 18, 13, 18),
                                       pge::Clip(-7, -18, 52, 18, 13, 18),
                                       pge::Clip(-7, -18.66, 65, 18, 13, 18) });
    Animation a_damage = Animation(3, std::vector<pge::Clip>{
                                          pge::Clip(-7, -18, 0, 18, 13, 18),
                                          pge::Clip(-7, -18, 13, 18, 13, 18) });
    Animation a_jump = Animation(1, std::vector<pge::Clip>{
                                        pge::Clip(-7, -18, 39, 36, 13, 18),
                                        pge::Clip(-7, -18, 52, 36, 13, 18),
                                        pge::Clip(-7, -18, 65, 36, 13, 18) });
    Animation a_ladder = Animation(1, std::vector<pge::Clip>{
                                          pge::Clip(-7, -18, 0, 36, 13, 18),
                                          pge::Clip(-7, -18, 13, 36, 13, 18),
                                          pge::Clip(-7, -18, 26, 36, 13, 18),
                                          pge::Clip(-7, -18, 13, 36, 13, 18) });
    Animation a_ladderhor = Animation(5, std::vector<pge::Clip>{
                                             pge::Clip(-7, -18, 0, 18, 13, 18),
                                             pge::Clip(-7, -18, 0, 18, 13, 18),
                                             pge::Clip(-7, -18, 0, 18, 13, 18),
                                             pge::Clip(-7, -18, 13, 18, 13, 18),
                                             pge::Clip(-7, -18, 13, 18, 13, 18),
                                             pge::Clip(-7, -18, 13, 36, 13, 18),
                                             pge::Clip(-7, -18, 13, 36, 13, 18),
                                         });
    Animation a_swim = Animation(10, std::vector<pge::Clip>{
                                         pge::Clip(-7, -18, 39, 54, 13, 18),
                                         pge::Clip(-7, -18, 39, 54, 13, 18),
                                         pge::Clip(-7, -18, 52, 54, 13, 18),
                                         pge::Clip(-7, -18, 52, 54, 13, 18),
                                         pge::Clip(-7, -18, 39, 54, 13, 18),
                                         pge::Clip(-7, -18, 65, 54, 13, 18),
                                         pge::Clip(-7, -18, 65, 54, 13, 18),
                                     });
    Animation* anim_active = &a_idle;

protected:
    friend class Controller;
    friend class PController;
    bool m_up = 0, m_left = 0, m_right = 0, m_down = 0, m_sprint = 0, m_jump = 0;
    bool m_hpdrain = 0, m_hpup = 0, m_endrain = 0, m_enup = 0;

    ~Player();
    void prelogic(uint16_t diff);
    void collision(Entity* target);
    void fixedstep();
    void logic(uint16_t diff);
    void draw(pge::viewport::View& vp);

public:
    Player(Controller* _source, float _x, float _y, uint16_t _life, PController* _pcontrol = nullptr);

    //No callback on delete;
    void destroy(uint16_t value)
    {
        this->flagDestroy = true;
    }
    void push(float _vx, float _vy);
    void take_damage(uint16_t value, Entity* source);
};

#endif
