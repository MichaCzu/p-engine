#include "app/menu/menu_class.hpp"
#include "pge/pge.hpp"

class MenuOptionsC : public MenuClass {
private:
    uint16_t font, font2, font3;
    int8_t selection = 0;
    float pos = 0;
    float siz = 35;

public:
    void reload()
    {
        pos = 0;
        selection = 0;
        flag = 255;
    }
    void load(uint16_t _font, uint16_t _font2)
    {
        font = _font;
        font2 = _font2;
    }
    void logic(SDL_Scancode key);
    uint8_t check(float fun);
    void draw(float fun);
};

void MenuOptionsC::logic(SDL_Scancode key)
{
    if (co_check(key, ec_up)) {
        if (selection > 0)
            selection--;
        else
            selection = 5;
        flag = 128;
    } else if (co_check(key, ec_down)) {
        if (selection < 5)
            selection++;
        else
            selection = 0;
        flag = 128;
    } else if (co_check(key, ec_left)) {
        //insert
    } else if (co_check(key, ec_right)) {
        //insert
    } else if (co_check(key, ec_apply)) {
        if (selection == 5 || selection == 4)
            flag = 2;
    } else if (co_check(key, ec_back)) {
        flag = 2;
    }
}

void MenuOptionsC::draw(float fun)
{
    int scale = pge::draw::get_scale();
    float targ;
    if (selection < 3)
        targ = selection * 38;
    else
        targ = (selection - 2) * 25 + 99;

    float in = (float)pge::get_time_difference() / 150.f * 150;
    if (pos > targ - in * 0.02 && pos < targ + in * 0.02)
        pos = targ;
    else if (pos < targ)
        pos += in * ((targ - pos) / 100 * 0.99 + 0.01);
    else
        pos -= in * ((pos - targ) / 100 * 0.99 + 0.01);

    if (selection < 3)
        targ = 36;
    else
        targ = 20;

    in = (float)pge::get_time_difference() / 150.f * 20;
    if (siz < targ + in && siz > targ - in)
        siz = targ;
    else if (siz < targ)
        siz += in;
    else
        siz -= in;

    int w, h;
    pge::window::get_size(&w, &h);
    int height = (236) * scale * fun;
    SDL_Color shcolor = { 0, 0, 0, fun * 0x57 },
              focolor = { 0xFF, 0xFF, 0xFF, fun * 0xFF },
              secolor = { 0xFF, 0xFF, 0xFF, fun * 0x57 };

    pge::draw::rectFill(w / 2 - 80 * scale, (h - height) / 2, 160 * scale, height, SDL_Color{ 0, 0, 0, 0x57 });
    int len = pge::text::get_width(font2, tr_get(et_moptions)) / 2;
    pge::text::draw(font2, w / 2 - len, h / 2 - 110 * scale * fun, tr_get(et_moptions), shcolor);
    pge::text::draw(font2, w / 2 - len, h / 2 - 112 * scale * fun, tr_get(et_moptions), focolor);
    pge::draw::rectFill(w / 2 - 80 * scale, h / 2 + (pos - 80 + (siz - 36) / 2) * scale * fun, 160 * scale, siz * scale * fun, secolor);

    len = pge::text::get_width(font, tr_get(et_mlanguage)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 78 * scale * fun, tr_get(et_mlanguage), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 80 * scale * fun, tr_get(et_mlanguage), focolor);
    len = pge::text::get_width(font3, L'<' + tr_get(et_msellang) + L'>') / 2;
    pge::text::draw(font3, w / 2 - len, h / 2 - 58.5 * scale * fun, L'<' + tr_get(et_msellang) + L'>', shcolor);
    pge::text::draw(font3, w / 2 - len, h / 2 - 60 * scale * fun, L'<' + tr_get(et_msellang) + L'>', focolor);

    len = pge::text::get_width(font, tr_get(et_mframerate)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 40 * scale * fun, tr_get(et_mframerate), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 42 * scale * fun, tr_get(et_mframerate), focolor);

    len = pge::text::get_width(font3, L'<' + tr_get(et_mdisabled) + L'>') / 2;
    pge::text::draw(font3, w / 2 - len, h / 2 - 20.5 * scale * fun, L'<' + tr_get(et_mdisabled) + L'>', shcolor);
    pge::text::draw(font3, w / 2 - len, h / 2 - 22 * scale * fun, L'<' + tr_get(et_mdisabled) + L'>', focolor);

    len = pge::text::get_width(font, tr_get(et_mfullscreen)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 2 * scale * fun, tr_get(et_mfullscreen), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 4 * scale * fun, tr_get(et_mfullscreen), focolor);
    len = pge::text::get_width(font3, L'<' + tr_get(et_mdisabled) + L'>') / 2;
    pge::text::draw(font3, w / 2 - len, h / 2 + 17.5 * scale * fun, L'<' + tr_get(et_mdisabled) + L'>', shcolor);
    pge::text::draw(font3, w / 2 - len, h / 2 + 16 * scale * fun, L'<' + tr_get(et_mdisabled) + L'>', focolor);

    len = pge::text::get_width(font, tr_get(et_mcontrols)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 37 * scale * fun, tr_get(et_mcontrols), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 35 * scale * fun, tr_get(et_mcontrols), focolor);

    len = pge::text::get_width(font, tr_get(et_mapply)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 62 * scale * fun, tr_get(et_mapply), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 60 * scale * fun, tr_get(et_mapply), focolor);

    len = pge::text::get_width(font, tr_get(et_mback)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 87 * scale * fun, tr_get(et_mback), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 85 * scale * fun, tr_get(et_mback), focolor);
}
