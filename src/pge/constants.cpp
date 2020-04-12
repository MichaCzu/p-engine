// Change this code to include new states
#include "pge/constants.hpp"
#include "app/state_game.hpp"
#include "app/state_intro.hpp"
#include "app/state_menu.hpp"
#include <stdint.h>

namespace pge {

State* get_state(uint8_t _id)
{
    switch (_id) {
    default:
    case 1:
        return new State_Intro;
    case 2:
        return new State_Menu;
    case 3:
        return new State_Game;
    }
}
}
