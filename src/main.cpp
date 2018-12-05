#include "pge/constants.hpp"
#include "pge/core.hpp"
#include "pge/debug.hpp"
#include "pge/draw.hpp"
#include <SDL2/SDL.h>
#include <iostream>

#include <vector>

int main(int argc, char* argv[])
{
    pge::init();
    pge::state::push(pge::es_intro);
    pge::handle_statecalls();

    //    float asd = 100;
    //    i_expect_int(&asd);

    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    // main game loop

    while (pge::is_running()) {
        //pge::handle_threadcalls();
        pge::handle_allevents();
        pge::update();
        pge::window::clear();
        pge::redraw();
        pge::window::render();
        pge::handle_statecalls();
        pge::delay_frame_difference();
    }

    //std::cout << "time: " << (clock_2 - clock_1) / double(CLOCKS_PER_SEC) * 1000 << ", ";

    // clearing session
    pge::close();

    return 0;
}
