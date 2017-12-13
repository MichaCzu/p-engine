#ifndef RPARTICLE_HPP
#define RPARTICLE_HPP

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

class Particle_Rain : public Particle {
protected:
    friend class Controller;

    uint16_t image;
    pge::Rect clip;

    ~Particle_Rain() {}
    void fixedlogic()
    {
        y += vy;
        x += vx;

        if ((vy > 0 && control->env()->bc_check((x) / 12, (y) / 12) < bt_platfm)) {
            vy = -0.05;
            if (life > 33) {
                life = 33;
                vx = 0;
                clip = clip = pge::Rect(0, 6 + pge::random::get(0, 1) * 2, 3, 2);
            }
        }

        life--;
        if (life <= 0)
            flagDestroy = true;
    }

    void draw(pge::viewport::View& vp)
    {
        int s = pge::draw::get_scale();
        pge::image::draw(image, (x - vp.x - clip.w / 2) * s, (y - vp.y - clip.h / 2) * s, &clip, NULL, 0, pge::draw::get_scale(), pge::draw::get_scale());
    }

    void set_pos(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

public:
    Particle_Rain(Controller* _control, float _x, float _y)
        : Particle(_control, _x, _y)
    {
        isfront = false;
        vx = 0.1;
        vy = 1;
        life = 3000;
        image = control->res()->get_image(5000, "res/spr/particle.png", 0xFF00FFFF);
        clip = pge::Rect(0 + pge::random::get(0, 2), 0, 1, 6);
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
