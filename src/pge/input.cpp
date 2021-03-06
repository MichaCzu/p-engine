#include "pge/input.hpp"
#include "pge/debug.hpp"
#include "pge/draw.hpp"
#include "pge/ini.hpp"
#include "pge/types.hpp"
#include <string>

namespace pge::input {

struct StructAxis {
    en_inputaxis id;
    std::wstring param;
    SDL_GameControllerAxis gaxis = SDL_CONTROLLER_AXIS_INVALID;
    SDL_Scancode axism = SDL_SCANCODE_UNKNOWN;
    SDL_Scancode axisp = SDL_SCANCODE_UNKNOWN;
    float state = 0.f;
};

struct StructControls {
    en_inputbtn id; //insert enum shortcut (not necessary)
    std::wstring param; //insert ini param to look for
    SDL_Scancode ctrlprim = SDL_SCANCODE_UNKNOWN; //insert default value
    SDL_GameControllerButton ctrlbtn = SDL_CONTROLLER_BUTTON_INVALID;
    SDL_Scancode ctrlalt = SDL_SCANCODE_UNKNOWN; //insert default value
    bool state = false;
};

StructAxis vAxis[] = {
	//{ ea_movex, L"move_x", SDL_CONTROLLER_AXIS_LEFTX, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT },
	//{ ea_movey, L"move_y", SDL_CONTROLLER_AXIS_LEFTY, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN },
    { ea_movex, L"move_x", SDL_CONTROLLER_AXIS_LEFTX, SDL_SCANCODE_A, SDL_SCANCODE_D },
    { ea_movey, L"move_y", SDL_CONTROLLER_AXIS_LEFTY, SDL_SCANCODE_W, SDL_SCANCODE_S },
    { ea_viewx, L"view_y", SDL_CONTROLLER_AXIS_RIGHTX },
    { ea_viewy, L"view_y", SDL_CONTROLLER_AXIS_RIGHTY },
    { ea_trigl, L"trigger_L", SDL_CONTROLLER_AXIS_TRIGGERLEFT, SDL_SCANCODE_Q },
    { ea_trigr, L"trigger_R", SDL_CONTROLLER_AXIS_TRIGGERRIGHT, SDL_SCANCODE_E }
};

StructControls vCtrls[] = {
    { ec_apply, L"apply", SDL_SCANCODE_RETURN, SDL_CONTROLLER_BUTTON_A },
    { ec_back, L"back", SDL_SCANCODE_ESCAPE, SDL_CONTROLLER_BUTTON_B },
    { ec_attack, L"attack", SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER },
    { ec_block, L"block", SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_LEFTSHOULDER },
    { ec_skill1, L"skill1", SDL_SCANCODE_1, SDL_CONTROLLER_BUTTON_DPAD_LEFT },
    { ec_skill2, L"skill2", SDL_SCANCODE_2, SDL_CONTROLLER_BUTTON_DPAD_UP },
    { ec_skill3, L"skill3", SDL_SCANCODE_3, SDL_CONTROLLER_BUTTON_DPAD_RIGHT },
    { ec_trinket, L"trinket", SDL_SCANCODE_R, SDL_CONTROLLER_BUTTON_DPAD_DOWN },
    { ec_item, L"item", SDL_SCANCODE_Q, SDL_CONTROLLER_BUTTON_X },
    { ec_menu, L"menu", SDL_SCANCODE_ESCAPE, SDL_CONTROLLER_BUTTON_START },
    { ec_inventory, L"inventory", SDL_SCANCODE_TAB, SDL_CONTROLLER_BUTTON_Y },
    { ec_interact, L"interact", SDL_SCANCODE_E, SDL_CONTROLLER_BUTTON_A },
    { ec_slowwalk, L"walk", SDL_SCANCODE_LSHIFT, SDL_CONTROLLER_BUTTON_LEFTSTICK, SDL_SCANCODE_RSHIFT },
    { ec_useless2, L"x", SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_RIGHTSTICK },
    { ec_uselessback, L"x", SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_BACK }
    //{ ec_, L"" },
};
bool lastEventMode = false;

float get_state(en_inputaxis _ctrl) { return vAxis[_ctrl].state; }
bool get_state(en_inputbtn _ctrl) { return vCtrls[_ctrl].state; }
bool alt_mode() { return lastEventMode; }
/*
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
*/

bool handle_input_event(SDL_Event& _evn)
{
    bool press = _evn.key.state == SDL_PRESSED ? 1 : 0;

    switch (_evn.type) {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        if (_evn.key.repeat != 0)
            break;
        lastEventMode = false;
        //keyboard button
        for (int i = 0; i < (sizeof(vCtrls) / sizeof(*vCtrls)); i++) {
            if (_evn.key.keysym.scancode == vCtrls[i].ctrlprim || _evn.key.keysym.scancode == vCtrls[i].ctrlalt)
                vCtrls[i].state = press;
        }
        //keyboard axis
        for (int i = 0; i < (sizeof(vAxis) / sizeof(*vAxis)); i++) {
            if (_evn.key.keysym.scancode == vAxis[i].axism)
                vAxis[i].state = press ? -1 : (vAxis[i].state != 1 ? 0 : 1);
            if (_evn.key.keysym.scancode == vAxis[i].axisp)
                vAxis[i].state = press ? 1 : (vAxis[i].state != -1 ? 0 : -1);
        }
        break;
    case SDL_CONTROLLERBUTTONUP:
    case SDL_CONTROLLERBUTTONDOWN:
        //controller button
        lastEventMode = true;
        for (int i = 0; i < (sizeof(vCtrls) / sizeof(*vCtrls)); i++) {
            if (_evn.cbutton.button == vCtrls[i].ctrlbtn) //
                vCtrls[i].state = _evn.cbutton.state == SDL_PRESSED ? 1 : 0;
        }
        break;
    case SDL_CONTROLLERAXISMOTION:
        //controller axis
        lastEventMode = true;
        for (int i = 0; i < (sizeof(vAxis) / sizeof(*vAxis)); i++) {
            if (_evn.caxis.axis == vAxis[i].gaxis)
                vAxis[i].state = (_evn.caxis.value + 1.0f) / 32767.f;
            else
                continue;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        lastEventMode = false;
    default:
        break;
    }
    return 0;
}
}

namespace input {
float state(en_inputaxis _ctrl) { return pge::input::get_state(_ctrl); }
bool state(en_inputbtn _ctrl) { return pge::input::get_state(_ctrl); }

std::vector<en_inputbtn> get(SDL_Scancode& _evn)
{
    std::vector<en_inputbtn> flag;
    for (auto& val : pge::input::vCtrls) {
        if (_evn == val.ctrlprim || val.ctrlalt == _evn) {
            flag.push_back(val.id);
            //pge::debug::log(std::string(val.param.begin(), val.param.end()));
        }
    }
    return flag;
}
}

namespace pge::mouse {

int x()
{
    int x, flag;
    SDL_GetMouseState(&x, &flag);
    return x;
}
int y()
{
    int y, flag;
    SDL_GetMouseState(&flag, &y);
    return y;
}
void state(int* x, int* y) { SDL_GetMouseState(x, y); }
pge::Pixel state()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return pge::Pixel(x, y);
}
void set(int x, int y) { SDL_WarpMouseInWindow(SDL_GetWindowFromID(window::get_target()->context->windowID), x, y); }
void set(pge::Pixel p) { SDL_WarpMouseInWindow(SDL_GetWindowFromID(window::get_target()->context->windowID), p.x, p.y); }
}
