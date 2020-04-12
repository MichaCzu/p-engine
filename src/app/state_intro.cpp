#include "app/state_intro.hpp"
#include "pge/core.hpp"
#include "pge/pge.hpp"
#include "pge/sound.hpp"
#include <iostream>
#include <stdint.h>

void State_Intro::init()
{
    //pge::crypt::file_ex("../res/spr/pgi/logo.pgs","../res/spr/pgi/logo.png");
    //pge::crypt::file_ex("../res/spr/pgi/spin.pgs","../res/spr/pgi/spin.png");
    //pge::crypt::file_ex("../res/spr/pgi/sout.pgs","../res/spr/pgi/sout.png");
    //pge::crypt::file_ex("../res/spr/pgi/pi.pgs","../res/spr/pgi/pi.png");
    //pge::crypt::file_ex("../res/spr/pgi/ilum.pgs","../res/spr/pgi/ilum.png");
    sLogo.set_filter(GPU_FILTER_LINEAR);
    sSpin.set_filter(GPU_FILTER_LINEAR);
    sSout.set_filter(GPU_FILTER_LINEAR);
    sPi.set_filter(GPU_FILTER_LINEAR);
    sIlum.set_filter(GPU_FILTER_LINEAR);
}

void State_Intro::cleanup()
{
    pge::music::purgeall();
    pge::image::purgeall();
}

void State_Intro::handle_events(SDL_Event _event)
{
    switch (_event.type) {
    case SDL_KEYDOWN:
    case SDL_CONTROLLERBUTTONDOWN:
        if (counter < 4000)
            counter = 5000;
        break;
    default:
        break;
    }
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
        //pge::debug::log("changing state to game...");
        pge::state::drop(pge::es_game);
    }

    pge::window::get_size(&w, &h);

    pge::Rect s1 = { 0, 0, 415, 360 };
    pge::Rect s2 = { 428, 38, 327, 284 };
    pge::Rect s3 = { 768, 77, 235, 205 };

    float scale = 0.5;
    if (h >= 1440 && w >= 2560)
        scale = 2;
    else if (h >= 720 && w >= 1280)
        scale = 1;

    int sLW = sLogo.get_width() / 2 * scale;
    int sPW = sPi.get_width() / 2 * scale;
    int sPH = sPi.get_height() / 2 * scale;
    int sIW = sIlum.get_width() / 2 * scale;
    int sIH = sIlum.get_height() / 2 * scale;

    //pge::media_setalpha_texture(sSpin, 255);

    sSpin.draw(w / 2 - s2.w * scale / 2 + 20 * scale, h / 2 - s2.h * scale / 2 - 150 * scale, &s2, NULL, -rotate / 16, scale, scale);
    sSpin.draw(w / 2 - s3.w * scale / 2 + 105 * scale, h / 2 - s3.h * scale / 2, &s3, NULL, rotate / 8, scale, scale);
    sSpin.draw(w / 2 - s1.w * scale / 2 - 135 * scale, h / 2 - s1.h * scale / 2 + 40 * scale, &s1, NULL, rotate / 24, scale, scale);

    sSout.draw(w / 2 - s1.w * scale / 2 - 135 * scale, h / 2 - s1.h * scale / 2 + 40 * scale, &s1, NULL, rotate / 24, scale, scale);
    sSout.draw(w / 2 - s2.w * scale / 2 + 20 * scale, h / 2 - s2.h * scale / 2 - 150 * scale, &s2, NULL, -rotate / 16, scale, scale);
    sSout.draw(w / 2 - s3.w * scale / 2 + 105 * scale, h / 2 - s3.h * scale / 2, &s3, NULL, rotate / 8, scale, scale);
    sIlum.draw(w / 2 - sIW - 45 * scale, h / 2 - sIH - 30 * scale, NULL, NULL, 0, scale, scale);
    sLogo.draw(w / 2 - sLW - 40 * scale, h / 2 + sPH - 80 * scale, NULL, NULL, 0, scale, scale);
    sPi.draw(w / 2 - sPW, h / 2 - sPH - 50 * scale, NULL, NULL, 0, scale, scale);
    pge::draw::rectF(w / 2 - 500 * scale, h / 2 - 500 * scale, 1000 * scale, 1000 * scale, SDL_Color{ 0, 0, 0, Uint8(255 - aPi) });
}
