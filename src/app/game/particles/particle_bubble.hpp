#ifndef BBPARTICLE_HPP
#define BBPARTICLE_HPP

#include "app/game/block.hpp"
#include "app/game/controller.hpp"
#include "app/game/particles/particle.hpp"
#include "pge/crypt.hpp"
#include "pge/debug.hpp"
#include "pge/draw.hpp"
#include "pge/image.hpp"
#include "pge/types.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

class Particle_Bubble : public Particle {
protected:
    friend class Controller;

    uint16_t image;
    pge::Rect clip;
    pge::Rect clipb;

    ~Particle_Bubble() {}
    void fixedlogic()
    {
        if (vy > -0.3)
            vy -= 0.01;

        if (vx != 0 && control->env()->bc_check((x + vx) / 12, (y) / 12) == bt_water)
            x += vx;

        if (vx != 0)
            vx += vx > 0 ? -0.01 : 0.01;

        if (vy < 0 && control->env()->bc_check((x + vx) / 12, (y + vy) / 12) != bt_water) {
            vy = 0;
            if (life > 20)
                life = 20;
        }

        y += vy;

        life--;
        if (life <= 0)
            flagDestroy = true;
    }

    void draw(pge::viewport::View& vp)
    {
        int s = pge::draw::get_scale();
        if (life > 20)
            pge::image::draw(image, (x - vp.x - clip.w / 2) * s, (y - vp.y - clip.h / 2) * s, &clip, NULL, 0, pge::draw::get_scale(), pge::draw::get_scale());
        else
            pge::image::draw(image, (x - vp.x - clip.w / 2) * s, (y - vp.y - clip.h / 2) * s, &clipb, NULL, 0, pge::draw::get_scale(), pge::draw::get_scale());
    }

    void set_pos(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

public:
    Particle_Bubble(Controller* _control, float _x, float _y, float _vx, float _vy, uint16_t lifeb = 0, uint16_t lifea = 0)
        : Particle(_control, _x, _y)
    {
        vx = _vx;
        vy = _vy;
        life = pge::random::get(30 - lifea, 200 - lifeb);
        int test = pge::random::get(1, 100);
        if (test < 60) {
            clip = pge::Rect(0, 7, 3, 3);
            clipb = pge::Rect(0, 10, 3, 3);
        } else {
            clip = pge::Rect(3, 0, 4, 4);
            clipb = pge::Rect(3, 4, 4, 4);
        }

        isfront = true;
        image = control->res()->get_image(5000, "res/spr/particle.png", 0xFF00FFFF);
    }

    void push(float _vx, float _vy) {}
    void destroy() { flagDestroy = true; }
    void get_pos(float* _x, float* _y)
    {
        *_x = x;
        *_y = y;
    }
};

#endif
