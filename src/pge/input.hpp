//managing game controls, getting text input.
#ifndef PGE_INPUT_HPP
#define PGE_INPUT_HPP

#include <SDL2/SDL.h>
#include <string>

enum en_contrls {
    ec_up = 0,
    ec_down,
    ec_left,
    ec_right,
    ec_apply,
    ec_back,
    ec_sprint,
    ec_jump,
    ec_shiftfor,
    ec_shiftbac,
    ec_actionpri,
    ec_actionsec,
    ec_inventory,
};
SDL_Scancode co_get(en_contrls _key, bool sec = 0);
bool co_check(SDL_Scancode _evn, en_contrls _key);

namespace pge::input {

bool load(std::string _path); //reads file with controls that overwrite defaults
//bool controls_load_defaults(std::string _path);
//bool controls_save(std::string _path); //reads file with controls that overwrite defaults
SDL_Scancode get(en_contrls _key, bool sec = 0);
std::string interpret(en_contrls _key);
//bool controls_set(en_contrls _key, bool sec = 0, SDL_Scancode _btn);
void resort();
}
#endif //TRANSLATOR_HPP
