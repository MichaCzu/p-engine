
#include "app/menu/menu_class.hpp"
#include "pge/pge.hpp"

class MenuMain : public MenuClass {
private:
    uint16_t font;
    uint16_t title;
    int8_t selection = 0;
    float pos = 0;

public:
    void reload()
    {
        pos = 0;
        selection = 0;
        flag = 255;
    }
    void load(uint16_t _font, uint16_t _title)
    {
        font = _font;
        title = _title;
    }
    void logic(SDL_Scancode key);
    void draw(float fun);
};

void MenuMain::logic(SDL_Scancode key)
{
    if (co_check(key, ec_up)) {
        if (selection > 0) {
            selection--;
            if (selection == 1)
                selection--;
        } else
            selection = 4;
        flag = 128;
    } else if (co_check(key, ec_down)) {
        if (selection < 4) {
            selection++;
            if (selection == 1)
                selection++;
        } else
            selection = 0;
        flag = 128;
    } else if (co_check(key, ec_apply)) {
        if (selection == 0)
            flag = 99;
        else
            flag = 4 - selection;
    } else if (co_check(key, ec_back)) {
        flag = 0;
    }
}

void MenuMain::draw(float fun)
{
    int scale = pge::draw::get_scale();

    float targ = selection * 25;
    float in = (float)pge::get_time_difference() / 150.f * 150;
    if (pos >= targ - in * 0.01 && pos < targ + in * 0.01)
        pos = targ;
    else if (pos <= targ)
        pos += in * ((targ - pos) / 100 * 0.99 + 0.01);
    else
        pos -= in * ((pos - targ) / 100 * 0.99 + 0.01);

    int w, h;
    pge::window::get_size(&w, &h);

    int height = (230) * scale * fun;
    SDL_Color shcolor = { 0, 0, 0, fun * 0x57 },
              focolor = { 0xFF, 0xFF, 0xFF, fun * 0xFF },
              secolor = { 0xFF, 0xFF, 0xFF, fun * 0x57 };

    pge::draw::rectFill(w / 2 - 80 * scale, (h - height) / 2, 160 * scale, height, SDL_Color{ 0, 0, 0, 0x57 });

    pge::image::set_colormod(title, shcolor);
    pge::image::draw(title, (w - 123 * scale) / 2, (h - height + 19 * scale * fun) / 2, NULL, NULL, 0, scale, scale * fun);
    pge::image::set_colormod(title, focolor);
    pge::image::draw(title, (w - 123 * scale) / 2, (h - height + 16 * scale * fun) / 2, NULL, NULL, 0, scale, scale * fun);

    pge::draw::rectFill(w / 2 - 80 * scale, h / 2 + (pos - 14) * scale * fun, 160 * scale, 20 * scale * fun, secolor);

    int len = pge::text::get_width(font, tr_get(et_mnewgame)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 13 * scale * fun, tr_get(et_mnewgame), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 15 * scale * fun, tr_get(et_mnewgame), focolor);

    len = pge::text::get_width(font, tr_get(et_mloadgame)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 12 * scale * fun, tr_get(et_mloadgame), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 10 * scale * fun, tr_get(et_mloadgame), secolor);

    len = pge::text::get_width(font, tr_get(et_moptions)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 37 * scale * fun, tr_get(et_moptions), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 35 * scale * fun, tr_get(et_moptions), focolor);

    len = pge::text::get_width(font, tr_get(et_mcredits)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 62 * scale * fun, tr_get(et_mcredits), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 60 * scale * fun, tr_get(et_mcredits), focolor);

    len = pge::text::get_width(font, tr_get(et_mquit)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 87 * scale * fun, tr_get(et_mquit), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 85 * scale * fun, tr_get(et_mquit), focolor);
}
