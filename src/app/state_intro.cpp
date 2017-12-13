#include "app/state_intro.hpp"
#include "app/menu/settings_man.hpp"
#include "pge/core.hpp"
#include "pge/pge.hpp"
#include <iostream>
#include <stdint.h>

void State_Intro::init()
{
    settings_load();
    pge::crypt::file("res/spr/pgi/logo.pgs");
    sLogo = pge::image::load("res/spr/pgi/logo.pgs");
    pge::image::set_filter(sLogo, GPU_FILTER_LINEAR);
    pge::crypt::file("res/spr/pgi/logo.pgs");

    pge::crypt::file("res/spr/pgi/spin.pgs");
    sSpin = pge::image::load("res/spr/pgi/spin.pgs");
    pge::image::set_filter(sSpin, GPU_FILTER_LINEAR);
    pge::crypt::file("res/spr/pgi/spin.pgs");

    pge::crypt::file("res/spr/pgi/sout.pgs");
    sSout = pge::image::load("res/spr/pgi/sout.pgs");
    pge::image::set_filter(sSout, GPU_FILTER_LINEAR);
    pge::crypt::file("res/spr/pgi/sout.pgs");

    pge::crypt::file("res/spr/pgi/pi.pgs");
    sPi = pge::image::load("res/spr/pgi/pi.pgs");
    pge::image::set_filter(sPi, GPU_FILTER_LINEAR);
    pge::crypt::file("res/spr/pgi/pi.pgs");

    pge::crypt::file("res/spr/pgi/ilum.pgs");
    sIlum = pge::image::load("res/spr/pgi/ilum.pgs");
    pge::image::set_filter(sIlum, GPU_FILTER_LINEAR);
    pge::crypt::file("res/spr/pgi/ilum.pgs");
}

void State_Intro::cleanup()
{
    pge::music::purgeall();
    pge::image::purgeall();
}

void State_Intro::handle_events(SDL_Event _event)
{
    if (_event.type == SDL_KEYDOWN && counter < 4000)
        counter = 5000;
}

void State_Intro::update()
{

    //Pi
    if (counter > tPi[ePi].s)
        ePi++;
    double value = (float)(counter - tPi[ePi - 1].s) / (float)(tPi[ePi].s - tPi[ePi - 1].s);
    if (tPi[ePi].t < tPi[ePi - 1].t)
        aPi = tPi[ePi - 1].t - (tPi[ePi - 1].t - tPi[ePi].t) * value;
    else
        aPi = tPi[ePi - 1].t + (tPi[ePi].t - tPi[ePi - 1].t) * value;

    if (aPi > 0)
        rotate += pge::get_time_difference() * (float)(aPi + 40) / 295.0;
}

void State_Intro::draw()
{
    if (counter < 5000)
        counter += pge::get_time_difference();
    if (counter >= 5000) {
        counter = 5000;
        std::cout << "changing state to menu..." << std::endl;
        pge::state::drop(pge::es_menu);
    }

    pge::window::get_size(&w, &h);

    pge::Rect s1 = { 0, 0, 415, 360 };
    pge::Rect s2 = { 428, 38, 327, 284 };
    pge::Rect s3 = { 768, 77, 235, 205 };

    float scale;
    if (h >= 1440 and w >= 2560)
        scale = 2;
    else if (h >= 720 and w >= 1280)
        scale = 1;
    else
        scale = 0.5;

    int sLW = pge::image::get_width(sLogo) / 2 * scale;
    int sPW = pge::image::get_width(sPi) / 2 * scale;
    int sPH = pge::image::get_height(sPi) / 2 * scale;
    int sIW = pge::image::get_width(sIlum) / 2 * scale;
    int sIH = pge::image::get_height(sIlum) / 2 * scale;

    //pge::media_setalpha_texture(sSpin, 255);

    pge::image::draw(sSpin, w / 2 - s2.w * scale / 2 + 20 * scale, h / 2 - s2.h * scale / 2 - 150 * scale, &s2, NULL, -rotate / 16, scale, scale);

    pge::image::draw(sSpin, w / 2 - s3.w * scale / 2 + 105 * scale, h / 2 - s3.h * scale / 2, &s3, NULL, rotate / 8, scale, scale);

    pge::image::draw(sSpin, w / 2 - s1.w * scale / 2 - 135 * scale, h / 2 - s1.h * scale / 2 + 40 * scale, &s1, NULL, rotate / 24, scale, scale);

    pge::image::draw(sSout, w / 2 - s1.w * scale / 2 - 135 * scale, h / 2 - s1.h * scale / 2 + 40 * scale, &s1, NULL, rotate / 24, scale, scale);
    pge::image::draw(sSout, w / 2 - s2.w * scale / 2 + 20 * scale, h / 2 - s2.h * scale / 2 - 150 * scale, &s2, NULL, -rotate / 16, scale, scale);
    pge::image::draw(sSout, w / 2 - s3.w * scale / 2 + 105 * scale, h / 2 - s3.h * scale / 2, &s3, NULL, rotate / 8, scale, scale);
    uint32_t color = 0;
    pge::image::draw(sIlum, w / 2 - sIW - 45 * scale, h / 2 - sIH - 30 * scale, NULL, NULL, 0, scale, scale);
    pge::image::draw(sLogo, w / 2 - sLW - 40 * scale, h / 2 + sPH - 80 * scale, NULL, NULL, 0, scale, scale);
    pge::image::draw(sPi, w / 2 - sPW, h / 2 - sPH - 50 * scale, NULL, NULL, 0, scale, scale);
    pge::draw::rectFill(w / 2 - 500 * scale, h / 2 - 500 * scale, 1000 * scale, 1000 * scale, SDL_Color{ 0, 0, 0, 255 - aPi });
}
