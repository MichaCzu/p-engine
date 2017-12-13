#ifndef PARTICLE_HPP
#define PARTICLE_HPP

/*
/ Entity types:
/ p - Player
/ f - Friendly
/ e - Enemy
/ n - Npc
/ v - enViroment
/ o - Other
*/
#include "pge/draw.hpp"
#include "pge/types.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

class Controller;

class Particle {
protected:
    friend class Controller;

    Controller* control;
    bool flagDestroy = 0;
    bool isfront;
    int16_t life = 0;
    float vx = 0, vy = 0;
    float x = 0, y = 0;

    virtual ~Particle() {}
    virtual void fixedlogic() {}
    virtual void draw(pge::viewport::View& vp) {}

    void set_pos(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

public:
    Particle(Controller* _control, float _x, float _y)
    {
        x = _x;
        y = _y;
        control = _control;
    }

    virtual void push(float _vx, float _vy) {}
    virtual void destroy() {}
    void get_pos(float* _x, float* _y)
    {
        *_x = x;
        *_y = y;
    }
};

#endif
