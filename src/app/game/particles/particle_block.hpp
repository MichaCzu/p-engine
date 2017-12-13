#ifndef BPARTICLE_HPP
#define BPARTICLE_HPP

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

class Particle_Block : public Particle {
protected:
    friend class Controller;

    uint16_t image;
    bool iswater;
    pge::Rect clip;

    ~Particle_Block() {}
    void fixedlogic()
    {
        static bool inwater = false;
        if (control->env()->bc_check((x) / 12, (y) / 12) == bt_water) {
            if (iswater && vy > 0)
                life = 0;
            if (!inwater) {
                inwater = true;
                vy = vy / 4;
                vx = vx / 4;
            }
        } else {
            inwater = false;
        }

        if (inwater ? vy < 0.3 : vy < 1)
            vy += inwater ? 0.005 : 0.025;
        else
            vy = inwater ? 0.3 : 1;

        if (vx != 0)
            vx += vx > 0 ? -0.002 : 0.002;

        if (vx != 0 && control->env()->bc_check((x + vx) / 12, (y) / 12) > bt_platfm)
            x += vx;
        //else if (vx < 0 && control->env()->bc_check((x + vx) / 12, (y) / 12) > bt_platfm)
        //    x += vx;

        if (vy < 0 && control->env()->bc_check((x) / 12, (y + vy) / 12) <= bt_platfm)
            vy = 0;
        else if (vy > 0 && control->block_move(&x, &y))
            vy = -abs(vy) / 2;

        y += vy;

        life--;

        if (life <= 0)
            flagDestroy = true;

        uint8_t size;
        if (life < 50)
            size = 1;
        else if (life < 100)
            size = 2;
        else if (life < 150)
            size = 3;
        else
            size = 4;

        clip.w = size;
        clip.h = size;
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
    Particle_Block(Controller* _control, float _x, float _y, float _vx, float _vy, Block _block, uint16_t lifeb = 0, uint16_t lifea = 0)
        : Particle(_control, _x, _y)
    {
        vx = (float)pge::random::get(-4, 4) * 0.1;
        vy = _vy - pge::random::get(-5, 0) * 0.1;
        life = pge::random::get(50 - lifea, 225 - lifeb);
        isfront = false;
        image = _block.image;
        iswater = (_block.type == bt_water);
        clip = pge::Rect(0 + pge::random::get(0, 32), 48 + pge::random::get(0, 32), 2, 2);
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
