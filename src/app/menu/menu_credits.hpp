#include "app/menu/menu_class.hpp"
#include "pge/pge.hpp"

class MenuCredits : public MenuClass {
private:
    uint16_t font;
    uint16_t font2;

public:
    void reload()
    {
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

void MenuCredits::logic(SDL_Scancode key)
{
    if (co_check(key, ec_back) || co_check(key, ec_apply)) {
        flag = 5;
    }
}

void MenuCredits::draw(float fun)
{
    int scale = pge::draw::get_scale();
    int w, h;
    pge::window::get_size(&w, &h);

    int height = 120 * scale * fun;
    SDL_Color shcolor = { 0, 0, 0, fun * 0x57 },
              focolor = { 0xFF, 0xFF, 0xFF, fun * 0xFF },
              secolor = { 0xFF, 0xFF, 0xFF, fun * 0x57 };

    pge::draw::rectFill(w / 2 - 80 * scale, (h - height) / 2, 160 * scale, height, SDL_Color{ 0, 0, 0, 0x57 });
    pge::draw::rectFill(w / 2 - 80 * scale, h / 2 + 31 * scale * fun, 160 * scale, 20 * scale * fun, secolor);

    int len = pge::text::get_width(font2, tr_get(et_mdeveloped)) / 2;
    pge::text::draw(font2, w / 2 - len, h / 2 - 43 * scale * fun, tr_get(et_mdeveloped), shcolor);
    pge::text::draw(font2, w / 2 - len, h / 2 - 45 * scale * fun, tr_get(et_mdeveloped), focolor);

    len = pge::text::get_width(font, L"DarkHex") / 2;
    pge::text::draw(font, w / 2 - len, h / 2 - 18 * scale * fun, L"DarkHex", shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 - 20 * scale * fun, L"DarkHex", focolor);

    len = pge::text::get_width(font, tr_get(et_mback)) / 2;
    pge::text::draw(font, w / 2 - len, h / 2 + 32 * scale * fun, tr_get(et_mback), shcolor);
    pge::text::draw(font, w / 2 - len, h / 2 + 30 * scale * fun, tr_get(et_mback), focolor);
}
