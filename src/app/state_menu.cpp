#include "app/state_menu.hpp"
#include "app/game/enviroment_controller.hpp"
#include "app/menu/menu_class.hpp"
#include "app/menu/menu_credits.hpp"
#include "app/menu/menu_main.hpp"
#include "app/menu/menu_options.hpp"
#include "app/menu/menu_optionsc.hpp"
#include "app/menu/menu_optionsg.hpp"
#include "app/menu/menu_optionss.hpp"
#include "app/menu/settings_man.hpp"
#include "pge/pge.hpp"
#include <iostream>
#include <stdint.h>

MenuOptions menu_options;
MenuMain menu_main;
MenuCredits menu_credits;
MenuOptionsG menu_optionsg;
MenuOptionsS menu_optionss;

void State_Menu::init()
{
    sfx1 = pge::chunk::load("res/sfx/ui/move.wav");
    sfx2 = pge::chunk::load("res/sfx/ui/select.wav");
    title = pge::image::load("res/spr/hud/menu/title.png");
    bg1 = pge::image::load("res/spr/bg/plains/0.png");
    bg2 = pge::image::load("res/spr/bg/plains/1.png");
    bg3 = pge::image::load("res/spr/bg/plains/2.png");
    bg4 = pge::image::load("res/spr/bg/plains/3.png");
    bg5 = pge::image::load("res/spr/bg/plains/4.png");
    bg6 = pge::image::load("res/spr/bg/skies/sky0.png");
    bgg = pge::image::load("res/spr/env/grass.png");
    bgr = pge::image::load("res/spr/env/rock.png");
    playerImg = pge::image::load("res/spr/player/player.png");

    pge::window::get_size(&w, &h);
    xx = w / 2;
    pge::random::set_seed(0);
    pge::draw::set_scale_detail(1);
    pge::draw::set_scale_factor(w + 359, h + 359, 360);
    fontt = pge::text::load("res/font/unifont.ttf", 18 * pge::draw::get_scale());
    fonts = pge::text::load("res/font/unifont.ttf", 14 * pge::draw::get_scale());
    fontf = pge::text::load("res/font/unifont.ttf", 24);
    fontb = pge::text::load("res/font/unifont.ttf", 26 * pge::draw::get_scale());
    pge::music::play(pge::music::load("res/mus/title.ogg"), -1, 1500);

    screen = 0;
    vp.x = 0;
    vp.y = 0;
    menu_main.load(fontt, title);
    menu_credits.load(fontt, fonts);
    menu_options.load(fontt, fonts);
    menu_optionsg.load(fontt, fonts);
    menu_optionss.load(fontt, fonts);
    menu_active = &menu_main;
    pge::random::set_seed();
    int a = pge::random::get(0, 3);

    for (int i = 0; i < 16; i++) {
        a = pge::random::get(0, 3);
        tileup[i][0] = pge::Rect(48 + 12 * a, 60, 12, 12);
    }
    for (int i = 0; i < 16; i++) {
        a = pge::random::get(0, 3);
        tileup[i][1] = pge::Rect(0 + 12 * a, 36, 12, 12);
    }
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 2; j++) {
            a = pge::random::get(0, 7);
            tileup[i][j + 2] = pge::Rect(a > 3 ? 0 + 12 * (a - 4) : 0 + 12 * a, a > 3 ? 48 : 60, 12, 12);
        }
    }
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            a = pge::random::get(0, 7);
            tilebot[i][j] = pge::Rect(a > 3 ? 0 + 12 * (a - 4) : 0 + 12 * a, a > 3 ? 48 : 60, 12, 12);
            a = pge::random::get(0, 7);
            tileup[i][j + 4] = pge::Rect(a > 3 ? 0 + 12 * (a - 4) : 0 + 12 * a, a > 3 ? 48 : 60, 12, 12);
        }
    }
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 2; j++) {
            a = pge::random::get(0, 99);
            if (a > 40 + j * 40)
                tilemidt[i][j] = 1;
            else
                tilemidt[i][j] = 0;
        }
    }
}

void State_Menu::cleanup()
{
}

//WORKS PERFECTLY
void pop_back_utf8(std::string& utf8)
{
    if (utf8.empty())
        return;

    auto cp = utf8.data() + utf8.size();
    while (--cp >= utf8.data() && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {
    }
    if (cp >= utf8.data())
        utf8.resize(cp - utf8.data());
}
//WORKS PERFECTLY
void State_Menu::handle_events(SDL_Event _event)
{

    if (_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        pge::window::get_size(&w, &h);
        pge::draw::set_scale_factor(w + 359, h + 359, 360);
        pge::text::resize(fontb, 26 * pge::draw::get_scale());
        pge::text::resize(fontt, 18 * pge::draw::get_scale());
        pge::text::resize(fonts, 14 * pge::draw::get_scale());
    } else if (_event.key.type == SDL_KEYDOWN && _event.key.repeat == 0) {
        if (fun == 1 || (co_check(_event.key.keysym.scancode, ec_apply) && co_check(_event.key.keysym.scancode, ec_back)))
            menu_active->logic(_event.key.keysym.scancode);
    }
    //INPUT TEST
    //if (_event.type == SDL_TEXTINPUT) {
    //    text += _event.text.text;
    //    std::cout << text << std::endl;
    //}
    //if (_event.key.type == SDL_KEYDOWN && _event.key.keysym.sym == SDLK_BACKSPACE && SDL_IsTextInputActive() && !text.empty()) {
    //    pop_back_utf8(text);
    //    std::cout << text << std::endl;
    //}
    //std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    //pge::text::draw(fontf, 4, 16, converter.from_bytes(text), 0x000000FF);
    //INPUT TEST
}
void State_Menu::update()
{
    xx += 0.025 * pge::get_time_difference();
    static float campos = 0;

    yy = sin(campos / 5000.0 * M_PI / 2) * 300 + 1300;

    if (campos > 5000)
        campos = 5000;
    else if (campos < 5000 && !quit)
        campos += (float)pge::get_time_difference();
    else if (quit)
        campos -= (float)pge::get_time_difference() * 2 / quit;

    float scale = pge::draw::get_scale();

    vp.w = w / scale;
    vp.h = h / scale;
    vp.x = xx - vp.w / 2;
    vp.y = yy - vp.h / 3 * 2;

    uint8_t flagg = menu_active->check(fun);
    if (flagg <= 100)
        pge::chunk::play(sfx2);
    else if (flagg == 128)
        pge::chunk::play(sfx1);

    if (flagg == 0) {
        quit = 1;
        pge::music::stop(1000);
    } else if (flagg == 99) {
        quit = 2;
        pge::music::stop(1000);
    } else if (flagg <= 5) {
        vscreen = flagg;
        targ += 2;
    }

    if (!quit) {
        if (alpha > 255)
            alpha = 255;
        else if (alpha < 255)
            alpha += (float)pge::get_time_difference() / 2500.0 * 255.0;

        if (time_passed < targ && time_passed > 1)
            time_passed += (float)pge::get_time_difference() / 400.f;
        else if (time_passed < targ && alpha > 0x11)
            time_passed += (float)pge::get_time_difference() / 750.f;
        if (time_passed > targ)
            fun = 1;
        else
            fun = fabs(sin(time_passed * M_PI / 2));

        if (pastpassed < fun && screen != vscreen) {
            screen = vscreen;
            switch (screen) {
            case 1:
                menu_active = &menu_credits;
                break;
            case 2:
                menu_active = &menu_options;
                break;
            case 3:
                menu_active = &menu_optionsg;
                break;
            case 4:
                menu_active = &menu_optionss;
                break;
            case 5:
                menu_active = &menu_main;
                break;
            default:
                break;
            }
            menu_active->reload();
        }
        pastpassed = fun;
    } else {
        if (alpha < 0) {
            alpha = 0;
            if (quit == 1) {
                settings_save();
                pge::quit();
            } else if (quit == 2) {
                pge::state::drop(pge::es_game);
            }
        } else if (alpha > 0)
            alpha -= (float)pge::get_time_difference() / 1000.0 * 255.0 / quit;

        if (time_passed > 1)
            time_passed = 1;
        if (time_passed > 0)
            time_passed -= (float)pge::get_time_difference() / 750.f;
        if (time_passed < 0)
            fun = 0;
        else
            fun = fabs(sin(time_passed * M_PI / 2));
    }
}

void State_Menu::draw()
{
    scale = pge::draw::get_scale();
    int ypos = 1600;

    EController::env_draw_background_sky(bg6, 0x5e7bd1ff, 0x5266ccff, pge::get_time_difference());
    EController::env_draw_background(bg5, 0.95, ypos, vp);
    EController::env_draw_background(bg4, 0.80, ypos, vp);
    EController::env_draw_background(bg3, 0.65, ypos, vp);
    EController::env_draw_background(bg2, 0.45, ypos, vp);
    EController::env_draw_background(bg1, 0.25, ypos, vp);

    float xddd = (w - 130 * scale) / 4;
    float y = pge::viewport::get_ymoved(1600, &vp);

    pge::Rect aa;
    xddd = (w - 180 * scale) / 4;
    switch ((int)floor(pge::get_time_passed() % 1000 / 250)) {
    case 0:
        aa = pge::Rect(26, 0, 13, 18);
        pge::image::draw(playerImg, xddd, (y - 18) * scale, &aa, NULL, 0, scale, scale);
        break;
    case 1:
        aa = pge::Rect(39, 0, 13, 18);
        pge::image::draw(playerImg, xddd, (y - 18.5) * scale, &aa, NULL, 0, scale, scale);
        break;
    case 2:
        aa = pge::Rect(52, 0, 13, 18);
        pge::image::draw(playerImg, xddd, (y - 18) * scale, &aa, NULL, 0, scale, scale);
        break;
    case 3:
        aa = pge::Rect(65, 0, 13, 18);
        pge::image::draw(playerImg, xddd, (y - 18.5) * scale, &aa, NULL, 0, scale, scale);
        break;
    }

    float xxx = fmod(vp.x, 192) * scale;
    for (int iiw = 0; iiw <= w / (192 * scale) + 1; iiw++) {
        //first 4 layer
        for (int i = 0; i < 16; i++)
            for (int j = 0; j < 6; j++)
                pge::image::draw(bgg, (iiw * 192 + i * 12) * scale - xxx, (y + j * 12 - 12) * scale, &tileup[i][j], NULL, 0, scale, scale);
        //mixed next 4 layers
        for (int i = 0; i < 16; i++)
            for (int j = 6; j < 8; j++)
                if (tilemidt[i][j - 6] == 1)
                    pge::image::draw(bgg, (iiw * 192 + i * 12) * scale - xxx, (y + j * 12 - 12) * scale, &tileup[i][j], NULL, 0, scale, scale);
                else
                    pge::image::draw(bgr, (iiw * 192 + i * 12) * scale - xxx, (y + j * 12 - 12) * scale, &tileup[i][j], NULL, 0, scale, scale);
        //first 4 layer
        for (int iih = 0; iih <= (h - (y + 84) * scale) / 48 * scale + 1; iih++)
            for (int i = 0; i < 16; i++)
                for (int j = 0; j < 4; j++)
                    pge::image::draw(bgr, (iiw * 192 + i * 12) * scale - xxx, (y + j * 12 + 84 + iih * 48) * scale, &tilebot[i][j], NULL, 0, scale, scale);
    }

    //env_draw_background(bg0, 0, ypos, vp, 0x3b3431ff);
    menu_active->draw(fun);

    pge::text::draw(fontf, 4, 4, std::to_wstring(pge::get_fps()), SDL_Color{ 255, 255, 255, 255 });
    if (alpha < 255) {
        uint32_t color;
        pge::draw::rectFill(0, 0, w, h, SDL_Color{ 0, 0, 0, 254 - alpha });
    }
}
