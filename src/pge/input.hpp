//managing game controls, getting text input.
#ifndef PGE_INPUT_HPP
#define PGE_INPUT_HPP

#include <SDL2/SDL.h>
#include <string>
#include <vector>

enum en_inputaxis {
    ea_movex = 0,
    ea_movey,
    ea_viewx,
    ea_viewy,
    ea_trigl,
    ea_trigr,
};

enum en_inputbtn {
    ec_apply,
    ec_back,
    ec_attack,
    ec_shiftfor,
    ec_shiftbac,
    ec_actionpri,
    ec_actionsec,
    ec_inventory,
};

namespace pge::input {

bool load(std::string _path); //reads file with controls that overwrite defaults
//bool controls_load_defaults(std::string _path);
//bool controls_save(std::string _path); //reads file with controls that overwrite defaults
//SDL_Scancode get(en_contrls _key, bool sec = 0);
//std::string interpret(en_inputbtn _key);
bool handle_input_event(SDL_Event& _evn);

float get_state(en_inputaxis _ctrl);
bool get_state(en_inputbtn _ctrl);
//bool controls_set(en_contrls _key, bool sec = 0, SDL_Scancode _btn);
void resort();
}

namespace input {
std::vector<en_inputbtn> get(SDL_Scancode& _evn); //only for key presses

float state(en_inputaxis _ctrl); // { return pge::input::get_state(_ctrl); }
bool state(en_inputbtn _ctrl); // { return pge::input::get_state(_ctrl); }
}

namespace pge::mouse {
int x();
int y();
void state(int* x, int* y);
void set(int x, int y); // { SDL_WarpMouseInWindow(SDL_Window * window, int x, int y); }
}
#endif //TRANSLATOR_HPP
