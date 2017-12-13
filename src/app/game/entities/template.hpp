#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

#include "app/game/entities/entity.hpp"
#include "pge/pge.hpp"

class _NAME_ : public Entity {
    uint16_t sprite;
    float v_speed = 0, h_speed = 0;

    void _jump(float hs);
    void _move(float hs);

    uint16_t chk_damage[4];

    //Animations;
    Animation a_idle = Animation(2, std::vector<pge::Clip>{
                                        pge::Clip(-7, -17, 0, 0, 13, 18),
                                        pge::Clip(-7, -17, 0, 0, 13, 18),
                                        pge::Clip(-7, -17, 0, 0, 13, 18),
                                        pge::Clip(-7, -17, 0, 0, 13, 18) });
    Animation* anim_active = &a_idle;

protected:
    friend class Controller;

    ~_NAME_();
    void prelogic(uint16_t diff);
    void collision(Entity* target);
    void fixedstep();
    void logic(uint16_t diff);
    void draw(pge::viewport::View& vp);

public:
    _NAME_(Controller* _source, float _x, float _y, uint16_t _life, PController* _pcontrol = nullptr);

    //No callback on delete;
    void destroy(uint16_t value)
    {
        this->flagDestroy = true;
    }
    void push(float _vx, float _vy);
    void take_damage(uint16_t value, Entity* source);
};

#endif
