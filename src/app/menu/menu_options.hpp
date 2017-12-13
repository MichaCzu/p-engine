#include "app/menu/menu_class.hpp"
#include "pge/pge.hpp"
#include <iostream>

class MenuOptions : public MenuClass {
private:
    uint16_t font, font2, font3;
    int8_t selection = 0;
    float pos = 0;
    float siz = 35;
    std::vector<std::string> lang_filename;
    std::vector<std::wstring> lang_string;
    uint8_t sellang = 0;

public:
    void reload()
    {
        pos = 0;
        selection = 0;
        flag = 255;
        //std::string langflag = settings_get_language();
        //for (int i = 0; i < lang_filename.size(); i++)
        //if (lang_filename[i] == langflag)
        //    sellang = i;
    }
    void load(uint16_t _font, uint16_t _font2)
    {
        font = _font;
        font2 = _font2;
        lang_filename = settings_get_languagelist();
        std::string langflag = settings_get_language();

        for (int i = 0; i < lang_filename.size(); i++) {
            uint8_t langf = pge::ini::open("data/lang/" + lang_filename[i]);
            lang_string.push_back(pge::ini::read(langf, L"HEADER", L"lang"));
            pge::ini::close(langf);
            if (lang_filename[i] == langflag) {
                sellang = i;
            }
        }
    }
    void logic(SDL_Scancode key);
    uint8_t check(float fun);
    void draw(float fun);
};

void MenuOptions::logic(SDL_Scancode key)
{
    if (co_check(key, ec_up)) {
        if (selection > 0) {
            selection--;
            if (selection == 3)
                selection--;
        } else
            selection = 4;
        flag = 128;
    } else if (co_check(key, ec_down)) {
        if (selection < 4) {
            selection++;
            if (selection == 3)
                selection++;
        } else
            selection = 0;
        flag = 128;
    } else if (co_check(key, ec_left)) {
        if (selection < 1) {
            if (sellang > 0)
                sellang--;
            else
                sellang = lang_filename.size() - 1;
            flag = 128;
        }
    } else if (co_check(key, ec_right)) {
        if (selection < 1) {
            if (sellang < lang_filename.size() - 1)
                sellang++;
            else
                sellang = 0;
            flag = 128;
        }
    } else if (co_check(key, ec_apply)) {
        if (selection == 0) {
            settings_set_language(lang_filename[sellang]);
            flag = 100;
        }
        if (selection == 4)
            flag = 5;
        else if (selection == 2)
            flag = 4;
        else if (selection == 1)
            flag = 3;
    } else if (co_check(key, ec_back)) {
        flag = 5;
    }
}

void MenuOptions::draw(float fun)
{
    int scale = pge::draw::get_scale();
    float targ;
    if (selection < 1)
        targ = 0;
    else
        targ = (selection - 1) * 25 + 49;

    float in = (float)pge::get_time_difference() / 150.f * 150;
    if (pos > targ - in * 0.02 && pos < targ + in * 0.02)
        pos = targ;
    else if (pos < targ)
        pos += in * ((targ - pos) / 100 * 0.99 + 0.01);
    else
        pos -= in * ((pos - targ) / 100 * 0.99 + 0.01);

    if (selection < 1)
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
    int height = (154) * scale * fun;

    pge::draw::rectFill(w / 2 - 80 * scale, (h - height) / 2, 160 * scale, height, SDL_Color{ 0, 0, 0, 0x57 });
    pge::draw::rectFill(w / 2 - 80 * scale, h / 2 + (pos - 68 + (siz - 36) / 2) * scale * fun, 160 * scale, siz * scale * fun, secolor);

    int len = pge::text::get_width(font, tr_get(et_mlanguage)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 66 * scale * fun, tr_get(et_mlanguage), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 68 * scale * fun, tr_get(et_mlanguage), focolor);
    len = pge::text::get_width(font2, L'⇐' + lang_string[sellang] + L'⇒') / 2;
    pge::text::draw(font2, w / 2 - len, h / 2 - 46.5 * scale * fun, L'⇐' + lang_string[sellang] + L'⇒', shcolor);
    pge::text::draw(font2, w / 2 - len, h / 2 - 48 * scale * fun, L'⇐' + lang_string[sellang] + L'⇒', focolor);

    len = pge::text::get_width(font, tr_get(et_mgraphics)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 26 * scale * fun, tr_get(et_mgraphics), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 28 * scale * fun, tr_get(et_mgraphics), focolor);

    len = pge::text::get_width(font, tr_get(et_msound)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 1 * scale * fun, tr_get(et_msound), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 3 * scale * fun, tr_get(et_msound), focolor);

    len = pge::text::get_width(font, tr_get(et_mcontrols)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 24 * scale * fun, tr_get(et_mcontrols), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 22 * scale * fun, tr_get(et_mcontrols), secolor);

    len = pge::text::get_width(font, tr_get(et_mback)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 49 * scale * fun, tr_get(et_mback), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 47 * scale * fun, tr_get(et_mback), focolor);
}
