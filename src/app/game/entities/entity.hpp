#ifndef ENTITY_HPP
#define ENTITY_HPP

/*
/ Entity types:
/ p - Player
/ f - Friendly
/ e - Enemy
/ n - Npc
/ v - enViroment
/ o - Other
/ i - item
*/
#include "pge/draw.hpp"
#include "pge/types.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

class Controller;
class Animation {
    uint16_t speed; //frames per second
    std::vector<pge::Clip> clip;
    uint16_t frame;
    int16_t time_left = 0;

public:
    Animation(uint16_t _speed, std::vector<pge::Clip> _sprites, int16_t fframe = 0)
    {
        speed = 1000 / _speed;
        clip = _sprites;
        frame = fframe;
    }

    bool animate(int time_passed)
    {
        int16_t time_ = time_passed + (time_passed >= 0 ? time_left : -time_left);
        time_left = abs(time_ % speed);
        int16_t flag = frame + ceil(time_ / speed);

        if (flag >= 0)
            frame = flag % clip.size();
        else
            frame = (clip.size() + flag) % clip.size();

        if (flag >= clip.size())
            return 1;
        else
            return 0;
    }

    pge::Clip* get()
    {
        return &clip[frame];
    }

    uint16_t get_frame()
    {
        return frame;
    }

    Animation* set(int fframe)
    {
        frame = fframe;
        return this;
    }
};

class Health {
    float value;
    float cooldown;
    float max;
    //std::map<int, int> regen;

public:
    float get() { return value; };
    void set(float val) { value = val; };
};

class Entity {
protected:
    friend class Controller;

    bool _isFree(int type, float xoff = 0, float yoff = 0);
    bool _isType(int type, float xoff = 0, float yoff = 0);
    float _getModSpeed(float xoff = 0, float yoff = 0);
    float _getModSpdSlow(float xoff = 0, float yoff = 0);
    float _getModSpdUp(float xoff = 0, float yoff = 0);
    float _getModJump(float xoff = 0, float yoff = 0);
    float _getModJumpSpd(float xoff = 0, float yoff = 0);
    float _getModBounce(float xoff = 0, float yoff = 0);
    float _getModFalldmg(float xoff = 0, float yoff = 0);

    Controller* control;
    bool flagDestroy = 0;
    char type;
    float life = 0;
    float vx = 0, vy = 0;
    float x = 0, y = 0;

    bool hitReady = 0;
    SDL_Rect hitBox;
    SDL_Rect dmgBox;

    virtual ~Entity() {}
    virtual void prelogic(uint16_t diff) {}
    virtual void collision(Entity* target) {}
    virtual void fixedstep() {}
    virtual void logic(uint16_t diff) {}
    virtual void draw(pge::viewport::View& vp) {}

    void set_pos(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

public:
    Entity(Controller* _control, float _x, float _y, uint16_t _life)
    {
        control = _control;
        x = _x;
        y = _y;
        life = _life;
    }

    virtual void push(float _vx, float _vy) {}
    virtual void take_damage(uint16_t value, Entity* source) {}
    virtual void destroy(uint16_t value) {}
    void get_pos(float* _x, float* _y)
    {
        *_x = x;
        *_y = y;
    }
};

#endif
