#include "pge/input.hpp"
#include "pge/ini.hpp"
#include <string>

SDL_Scancode co_get(en_contrls _key, bool sec) { return pge::input::get(_key, sec); }
bool co_check(SDL_Scancode _evn, en_contrls _key)
{
    if (_evn == co_get(_key) || _evn == co_get(_key, 1))
        return true;
    else
        return false;
}

namespace pge::input {

struct StructControls {
    uint16_t id; //insert enum shortcut (not necessary)
    std::wstring param; //insert ini param to look for
    SDL_Scancode ctrl1 = SDL_SCANCODE_UNKNOWN; //insert default value
    SDL_Scancode ctrl2 = SDL_SCANCODE_UNKNOWN; //insert default value
};

StructControls vCtrls[] = {
    { ec_up, L"up", SDL_SCANCODE_UP },
    { ec_down, L"down", SDL_SCANCODE_DOWN },
    { ec_left, L"left", SDL_SCANCODE_LEFT },
    { ec_right, L"right", SDL_SCANCODE_RIGHT },
    { ec_apply, L"apply", SDL_SCANCODE_Z, SDL_SCANCODE_RETURN },
    { ec_back, L"back", SDL_SCANCODE_X },
    { ec_sprint, L"sprint", SDL_SCANCODE_LSHIFT },
    { ec_jump, L"jump", SDL_SCANCODE_UP },
    { ec_shiftfor, L"shift_forward", SDL_SCANCODE_S },
    { ec_shiftbac, L"shift_backward", SDL_SCANCODE_A },
    { ec_actionpri, L"action_primary", SDL_SCANCODE_Z },
    { ec_actionsec, L"action_secondary", SDL_SCANCODE_X },
    { ec_inventory, L"inventory", SDL_SCANCODE_F },
    //{ ec_, L"" },
};

bool load(std::string _path)
{
    int a = ini::open(_path);
    if (a != 255) {
        for (int i = 0; i < (sizeof(vCtrls) / sizeof(*vCtrls)); i++) {
            uint16_t buffer = ini::read_int(a, L"PRIMARY", vCtrls[i].param);
            if (buffer != 0)
                vCtrls[i].ctrl1 = static_cast<SDL_Scancode>(buffer);
            buffer = ini::read_int(a, L"SECONDARY", vCtrls[i].param);
            if (buffer != 0)
                vCtrls[i].ctrl2 = static_cast<SDL_Scancode>(buffer);
        }
        return true;
    } else
        return false;
}

SDL_Scancode get(en_contrls _key, bool sec)
{
    if (!sec)
        return vCtrls[_key].ctrl1;
    else
        return vCtrls[_key].ctrl2;
}

std::string interpret(en_contrls _key) { return SDL_GetKeyName(SDL_GetKeyFromScancode(co_get(_key))); }

void resort()
{
    for (int i = 1; i < (sizeof(vCtrls) / sizeof(*vCtrls)) - 1; i++) {
        for (int j = 0; j < (sizeof(vCtrls) / sizeof(*vCtrls)) - 1; j++) {
            if (vCtrls[j].id > vCtrls[j + 1].id) {
                StructControls buffer = vCtrls[j];
                vCtrls[j] = vCtrls[j + 1];
                vCtrls[j + 1] = buffer;
            }
        }
    }
}
}
