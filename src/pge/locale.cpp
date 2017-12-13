#include "pge/locale.hpp"
#include "pge/ini.hpp"
#include <string>

std::wstring tr_get(en_transl _key) { return pge::locale::get(_key); }

namespace pge::locale {

struct StructTranslation {
    uint16_t id; //insert enum shortcut (not necessary)
    std::wstring section; //insert ini section to look for
    std::wstring param; //insert ini param to look for
    std::wstring defaultt; //insert default value
    std::wstring transl; //insert default value
};

StructTranslation vTrans[] = {
    { et_msellang, L"HEADER", L"lang" },
    { et_mnewgame, L"MAIN_MENU", L"new_game" },
    { et_mloadgame, L"MAIN_MENU", L"load_game" },
    { et_moptions, L"MAIN_MENU", L"options" },
    { et_mcredits, L"MAIN_MENU", L"credits" },
    { et_mquit, L"MAIN_MENU", L"quit" },
    { et_mdeveloped, L"MAIN_MENU", L"developed" },
    { et_mlanguage, L"OPTIONS", L"language" },
    { et_mgraphics, L"OPTIONS", L"graphics" },
    { et_msound, L"OPTIONS", L"sound", L"Sound" },
    { et_mcontrols, L"OPTIONS", L"controls" },
    { et_mapply, L"OPTIONS", L"apply" },
    { et_mback, L"OPTIONS", L"back" },
    { et_mmusic, L"OPTIONS", L"music" },
    { et_mframerate, L"OPTIONS", L"framerate" },
    { et_mresolution, L"OPTIONS", L"resolution" },
    { et_mfullscreen, L"OPTIONS", L"fullscreen" },
    { et_menabled, L"OPTIONS", L"enabled" },
    { et_mdisabled, L"OPTIONS", L"disabled" },
    { et_mquality, L"OPTIONS", L"quality" },
    { et_mhigh, L"OPTIONS", L"high" },
    { et_mmedium, L"OPTIONS", L"medium" },
    { et_mlow, L"OPTIONS", L"low" },
    //{ et_m, L"", L"", L"" },
};

bool load(std::string _path)
{
    int a = ini::open(_path);
    if (a != 255) {
        for (int i = 0; i < (sizeof(vTrans) / sizeof(*vTrans)); i++) {
            std::wstring buffer = ini::read(a, vTrans[i].section, vTrans[i].param);
            if (buffer != L"")
                vTrans[i].transl = buffer;
            else
                vTrans[i].transl = vTrans[i].defaultt;
        }
        return true;
    } else
        return false;
}

bool setfallback(std::string _path)
{
    int a = ini::open(_path);
    if (a != 255) {
        for (int i = 0; i < (sizeof(vTrans) / sizeof(*vTrans)); i++) {
            std::wstring buffer = ini::read(a, vTrans[i].section, vTrans[i].param);
            if (buffer != L"")
                vTrans[i].defaultt = buffer;
        }
        return true;
    } else
        return false;
}

std::wstring get(en_transl _key)
{
    return vTrans[_key].transl;
}

void resort()
{
    for (int i = 1; i < (sizeof(vTrans) / sizeof(*vTrans)) - 1; i++) {
        for (int j = 0; j < (sizeof(vTrans) / sizeof(*vTrans)) - 1; j++) {
            if (vTrans[j].id > vTrans[j + 1].id) {
                StructTranslation buffer = vTrans[j];
                vTrans[j] = vTrans[j + 1];
                vTrans[j + 1] = buffer;
            }
        }
    }
}
}
