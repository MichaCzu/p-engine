#ifndef MENU_CLASS
#define MENU_CLASS

#include "app/menu/settings_man.hpp"
static uint8_t INIid;
static uint8_t quality;

bool settings_load()
{
    INIid = pge::ini::open("data/settings.ini");

    std::wstring slang = pge::ini::read(INIid, L"LANGUAGE", L"language");

    pge::set_defaultfps(pge::ini::read_int(INIid, L"GRAPHICS", L"fps"));
    pge::window::set_size(pge::ini::read_int(INIid, L"GRAPHICS", L"default_window_w"), pge::ini::read_int(INIid, L"GRAPHICS", L"default_window_h"));

    std::string strslang(slang.begin(), slang.end());
    pge::locale::load("data/lang/en_us.lang");
    pge::chunk::set_volume_all(pge::ini::read_int(INIid, L"VOLUME", L"sound_vol") * 12.8);
    pge::music::set_volume(pge::ini::read_int(INIid, L"VOLUME", L"music_vol") * 12.8);

    //if (pge::ini::read_int(INIid, L"GRAPHICS", L"fullscreen"))
    //        SDL_SetWindowFullscreen(pge::window::get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
    //    else
    //        SDL_SetWindowFullscreen(pge::window::get(), 0);

    if (!pge::locale::setfallback("data/lang/en_us.lang")) {
        pge::quit();
        pge::debug::log("Couldn't load default translation en_us");
        return 0;
    }

    return 1;
}

bool settings_save()
{
    pge::ini::save(INIid, "data/settings.ini");
    return 0;
}

void settings_set_soundvolume(uint8_t _sv)
{
    pge::chunk::set_volume_all(_sv * 12.8);
    pge::ini::write(INIid, L"VOLUME", L"sound_vol", std::to_wstring((int)_sv));
}
uint8_t settings_get_soundvolume() { return pge::ini::read_int(INIid, L"VOLUME", L"sound_vol"); }

void settings_set_musicvolume(uint8_t _mv)
{
    pge::music::set_volume(_mv * 12.8);
    pge::ini::write(INIid, L"VOLUME", L"music_vol", std::to_wstring((int)_mv));
}
uint8_t settings_get_musicvolume() { return pge::ini::read_int(INIid, L"VOLUME", L"music_vol"); }

void settings_set_language(std::string lang)
{
    std::wstring ws;
    ws.assign(lang.begin(), lang.end());
    pge::locale::load("data/lang/" + lang);
    pge::ini::write(INIid, L"LANGUAGE", L"language", ws);
}
std::string settings_get_language()
{
    std::wstring slang = pge::ini::read(INIid, L"LANGUAGE", L"language");
    std::string strslang(slang.begin(), slang.end());
    return strslang;
}
std::vector<std::string> settings_get_languagelist() { return pge::file::get_folderlist("data/lang/"); }

void settings_set_quality(uint8_t _q) { pge::ini::write(INIid, L"GRAPHICS", L"quality", std::to_wstring((int)_q)); }
uint8_t settings_get_quality() { return pge::ini::read_int(INIid, L"GRAPHICS", L"quality"); }
void settings_set_fullscreen(bool _fs) { pge::ini::write(INIid, L"GRAPHICS", L"fullscreen", std::to_wstring((int)_fs)); }
void settings_set_fps(uint16_t _fps) { pge::ini::write(INIid, L"GRAPHICS", L"fps", std::to_wstring((int)_fps)); }

void settings_set_controls();
void settings_get_controls();

#endif
