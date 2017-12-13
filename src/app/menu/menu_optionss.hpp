#include "app/menu/menu_class.hpp"
#include "pge/pge.hpp"

class MenuOptionsS : public MenuClass {
private:
    uint16_t font, font2;
    int8_t selection = 0;
    float pos = 0;
    float siz = 35;
    uint8_t mvol = 10, svol = 10;
    std::wstring prep_slider(int8_t);

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
        mvol = settings_get_musicvolume();
        svol = settings_get_soundvolume();
    }
    void logic(SDL_Scancode key);
    uint8_t check(float fun);
    void draw(float fun);
};

void MenuOptionsS::logic(SDL_Scancode key)
{
    if (co_check(key, ec_up)) {
        if (selection > 0)
            selection--;
        else
            selection = 2;
        flag = 128;
    } else if (co_check(key, ec_down)) {
        if (selection < 2)
            selection++;
        else
            selection = 0;
        flag = 128;
    } else if (co_check(key, ec_left)) {
        if (selection == 1 && svol > 0) {
            svol--;
            flag = 128;
            settings_set_soundvolume(svol);
        } else if (selection == 0 && mvol > 0) {
            mvol--;
            flag = 128;
            settings_set_musicvolume(mvol);
        }
    } else if (co_check(key, ec_right)) {
        if (selection == 1 && svol < 10) {
            svol++;
            flag = 128;
            settings_set_soundvolume(svol);
        } else if (selection == 0 && mvol < 10) {
            mvol++;
            flag = 128;
            settings_set_musicvolume(mvol);
        }
    } else if (co_check(key, ec_apply)) {
        if (selection == 2)
            flag = 2;
    } else if (co_check(key, ec_back)) {
        flag = 2;
    }
}

std::wstring MenuOptionsS::prep_slider(int8_t l)
{
    std::wstring buffer = L"├";
    for (int i = l; i > 0; i--) {
        buffer.push_back(L'─');
    }
    buffer.push_back(L'┴');
    for (int i = 0; i < 10 - l; i++) {
        buffer.push_back(L'─');
    }
    buffer.push_back(L'┤');

    return buffer;
}

void MenuOptionsS::draw(float fun)
{
    int scale = pge::draw::get_scale();
    float targ;
    if (selection < 2)
        targ = selection * 40;
    else
        targ = (selection - 2) * 25 + 89;

    float in = (float)pge::get_time_difference() / 150.f * 150;
    if (pos > targ - in * 0.02 && pos < targ + in * 0.02)
        pos = targ;
    else if (pos < targ)
        pos += in * ((targ - pos) / 100 * 0.99 + 0.01);
    else
        pos -= in * ((pos - targ) / 100 * 0.99 + 0.01);

    if (selection < 2)
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
    SDL_Color shcolor = { 0, 0, 0, fun * 0x57 },
              focolor = { 0xFF, 0xFF, 0xFF, fun * 0xFF },
              secolor = { 0xFF, 0xFF, 0xFF, fun * 0x57 };
    int height = (120) * scale * fun;

    pge::draw::rectFill(w / 2 - 80 * scale, (h - height) / 2, 160 * scale, height, SDL_Color{ 0, 0, 0, 0x57 });
    pge::draw::rectFill(w / 2 - 80 * scale, h / 2 + (pos - 50 + (siz - 36) / 2) * scale * fun, 160 * scale, siz * scale * fun, secolor);

    int len = pge::text::get_width(font, tr_get(et_mmusic)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 48 * scale * fun, tr_get(et_mmusic), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 50 * scale * fun, tr_get(et_mmusic), focolor);
    std::wstring slider = prep_slider(mvol);
    len = pge::text::get_width(font2, slider) / 2;
    pge::text::draw(font2, w / 2 - len, h / 2 - 28.5 * scale * fun, slider, shcolor);
    pge::text::draw(font2, w / 2 - len, h / 2 - 30 * scale * fun, slider, focolor);

    len = pge::text::get_width(font, tr_get(et_msound)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 8 * scale * fun, tr_get(et_msound), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 10 * scale * fun, tr_get(et_msound), focolor);
    slider = prep_slider(svol);
    len = pge::text::get_width(font2, slider) / 2;
    pge::text::draw(font2, w / 2 - len, h / 2 + 11.5 * scale * fun, slider, shcolor);
    pge::text::draw(font2, w / 2 - len, h / 2 + 10 * scale * fun, slider, focolor);

    len = pge::text::get_width(font, tr_get(et_mback)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 32 * scale * fun, tr_get(et_mback), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 30 * scale * fun, tr_get(et_mback), focolor);
}
