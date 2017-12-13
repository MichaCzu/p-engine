#include "app/state_game.hpp"
#include "app/game/controller.hpp"
#include "app/game/enviroment_controller.hpp"
#include "app/menu/settings_man.hpp"
#include "pge/core.hpp"
#include "pge/pge.hpp"
#include <iostream>
#include <stdint.h>

void State_Game::init()
{
    fontf = pge::text::load("res/font/unifont.ttf", 24);
    pge::draw::set_scale_detail(5);
    pge::window::get_size(&w, &h);
    pge::draw::set_scale_factor(w + 359, h + 359, 360);

    control = new Controller;
}

void State_Game::cleanup()
{
    delete control;
}

void State_Game::handle_events(SDL_Event _event)
{
    if (_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        pge::window::get_size(&w, &h);
        pge::draw::set_scale_factor(w + 359, h + 359, 360);
        control->adjust_draw();
    }
    control->handle_event(_event);
}

void State_Game::update()
{
    //    static float vx = 0, vy = 0;
    //float scale = pge::draw::get_scale();

    control->handle_prelogic();

    control->handle_destroy();
    control->handle_create();

    control->handle_collision();
    control->handle_fixedstep();
    control->handle_logic();
}

void State_Game::draw()
{
    control->handle_camera();

    control->handle_draw_background();
    //pge::image::set_target(control->fronttexture);
    //pge::window::clear();
    control->handle_draw();
    //pge::image::set_target(control->fronttexture);
    control->handle_draw_light();
    //pge::image::reset_target();
    //pge::image::draw(control->fronttexture, 0, 0);
    control->handle_draw_gui();

    pge::text::draw(fontf, 4, 4, std::to_wstring(pge::get_fps()), SDL_Color{ 255, 255, 255, 255 });

    if (alpha < 254) {
        pge::draw::rectFill(0, 0, w, h, SDL_Color{ 0, 0, 0, 254 - alpha });
        alpha += (float)pge::get_time_difference() / 3000.0 * 255.0;
    } else if (alpha > 254)
        alpha = 254;
}
