#include "app/menu/menu_class.hpp"
#include "pge/pge.hpp"

static std::wstring changeQuality(int val);
static std::wstring changeFramerate(int fps);
static std::wstring changeFullscreen(int val);

class MenuOptionsG : public MenuClass {
private:
    uint16_t font, font2;
    int selection = 0;
    float pos = 0;
    float siz = 35;
    int selQuality = 2, selFramerate = 0;
    int selFullscreen = 0;
    std::wstring sFramerate, sQuality, sFullscreen;

    std::wstring GetFramerate();
    std::wstring GetQuality;

public:
    void reload()
    {
        pos = 0;
        selection = 0;
        flag = 255;
        selFullscreen = pge::window::is_fullscreen();
        sFullscreen = changeFullscreen(selFullscreen);
        sFramerate = changeFramerate(pge::get_defaultfps());
    }
    void load(uint16_t _font, uint16_t _font2)
    {
        selFullscreen = pge::window::is_fullscreen();
        sFullscreen = changeFullscreen(selFullscreen);
        sFramerate = changeFramerate(pge::get_defaultfps());
        sQuality = changeQuality(selQuality);
        font = _font;
        font2 = _font2;
    }
    void logic(SDL_Scancode key);
    uint8_t check(float fun);
    void draw(float fun);
};

static std::wstring changeQuality(int val)
{
    switch (val) {
    case 0:
        settings_set_quality(0);
        return tr_get(et_mlow);
        break;
    default:
    case 1:
        settings_set_quality(1);
        return tr_get(et_mmedium);
        break;
    case 2:
        settings_set_quality(2);
        return tr_get(et_mhigh);
        break;
    }
}
static std::wstring changeFramerate(int fps)
{
    switch (fps) {
    case 5:
        pge::set_defaultfps(24);
        settings_set_fps(pge::get_defaultfps());
        return L"RETRO 24";
        break;
    case 6:
        pge::set_defaultfps(16);
        settings_set_fps(pge::get_defaultfps());
        return L"TRAGIC 16";
        break;
    case 7:
        pge::set_defaultfps(9);
        settings_set_fps(pge::get_defaultfps());
        return L"IMPOSSIBLE 9";
        break;
    case 4:
        pge::set_defaultfps(30);
        settings_set_fps(pge::get_defaultfps());
        return std::to_wstring(30) + L"fps";
        break;
    case 3:
        pge::set_defaultfps(60);
        settings_set_fps(pge::get_defaultfps());
        return std::to_wstring(60) + L"fps";
        break;
    case 2:
        pge::set_defaultfps(120);
        settings_set_fps(pge::get_defaultfps());
        return std::to_wstring(120) + L"fps";
        break;
    case 1:
        pge::set_defaultfps(240);
        settings_set_fps(pge::get_defaultfps());
        return std::to_wstring(240) + L"fps";
        break;
    case 0:
        pge::set_defaultfps(0);
        settings_set_fps(pge::get_defaultfps());
        return tr_get(et_mdisabled);
        break;
    default:
        pge::set_defaultfps(fps);
        settings_set_fps(pge::get_defaultfps());
        return std::to_wstring(fps) + L"fps";
        break;
    }
}
static std::wstring changeFullscreen(int val)
{
    if (val) {
        pge::window::set_fullscreen(1);
        settings_set_fullscreen(1);
        return tr_get(et_menabled);
    }
    pge::window::set_fullscreen(0);
    settings_set_fullscreen(0);
    return tr_get(et_mdisabled);
}

void MenuOptionsG::logic(SDL_Scancode key)
{
    if (co_check(key, ec_up)) {
        decrement(&selection, 3);
        flag = 128;
    } else if (co_check(key, ec_down)) {
        increment(&selection, 3);
        flag = 128;
    } else if (co_check(key, ec_left)) {
        switch (selection) {
        case 0:
            decrement(&selQuality, 2);
            sQuality = changeQuality(selQuality);
            break;
        case 1:
            decrement(&selFullscreen, 1);
            sFullscreen = changeFullscreen(selFullscreen);
            break;
        case 2:
            decrement(&selFramerate, 4);
            sFramerate = changeFramerate(selFramerate);
            break;
        }
        if (selection < 3)
            flag = 128;
    } else if (co_check(key, ec_right)) {
        switch (selection) {
        case 0:
            increment(&selQuality, 2);
            sQuality = changeQuality(selQuality);
            break;
        case 1:
            increment(&selFullscreen, 1);
            sFullscreen = changeFullscreen(selFullscreen);
            break;
        case 2:
            increment(&selFramerate, 7);
            sFramerate = changeFramerate(selFramerate);
            break;
        }
        if (selection < 3)
            flag = 128;
    } else if (co_check(key, ec_apply)) {
        if (selection == 3)
            flag = 2;
    } else if (co_check(key, ec_back)) {
        flag = 2;
    }

    selFullscreen = pge::window::is_fullscreen();
    sFullscreen = changeFullscreen(selFullscreen);
}

void MenuOptionsG::draw(float fun)
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
    int height = (156) * scale * fun;
    SDL_Color shcolor = { 0, 0, 0, fun * 0x57 },
              focolor = { 0xFF, 0xFF, 0xFF, fun * 0xFF },
              secolor = { 0xFF, 0xFF, 0xFF, fun * 0x57 };

    pge::draw::rectFill(w / 2 - 80 * scale, (h - height) / 2, 160 * scale, height, SDL_Color{ 0x00, 0x00, 0x00, 0x57 });
    pge::draw::rectFill(w / 2 - 80 * scale, h / 2 + (pos - 70 + (siz - 36) / 2) * scale * fun, 160 * scale, siz * scale * fun, secolor);

    int len = pge::text::get_width(font, tr_get(et_mquality)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 68 * scale * fun, tr_get(et_mquality), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 70 * scale * fun, tr_get(et_mquality), focolor);
    len = pge::text::get_width(font2, L'<' + sQuality + L'>') / 2;
    pge::text::draw(font2, w / 2 - len, h / 2 - 48.5 * scale * fun, L'<' + sQuality + L'>', shcolor);
    pge::text::draw(font2, w / 2 - len, h / 2 - 50 * scale * fun, L'<' + sQuality + L'>', focolor);

    len = pge::text::get_width(font, tr_get(et_mfullscreen)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 30 * scale * fun, tr_get(et_mfullscreen), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 32 * scale * fun, tr_get(et_mfullscreen), focolor);
    len = pge::text::get_width(font2, L'<' + sFullscreen + L'>') / 2;
    pge::text::draw(font2, w / 2 - len, h / 2 - 10.5 * scale * fun, L'<' + sFullscreen + L'>', shcolor);
    pge::text::draw(font2, w / 2 - len, h / 2 - 12 * scale * fun, L'<' + sFullscreen + L'>', focolor);

    len = pge::text::get_width(font, tr_get(et_mframerate)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 8 * scale * fun, tr_get(et_mframerate), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 6 * scale * fun, tr_get(et_mframerate), focolor);
    len = pge::text::get_width(font2, L'<' + sFramerate + L'>') / 2;
    pge::text::draw(font2, w / 2 - len, h / 2 + 27.5 * scale * fun, L'<' + sFramerate + L'>', shcolor);
    pge::text::draw(font2, w / 2 - len, h / 2 + 26 * scale * fun, L'<' + sFramerate + L'>', focolor);

    len = pge::text::get_width(font, tr_get(et_mback)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 48 * scale * fun, tr_get(et_mback), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 46 * scale * fun, tr_get(et_mback), focolor);
}
