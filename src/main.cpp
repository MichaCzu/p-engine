#include "pge/constants.hpp"
#include "pge/core.hpp"
#include "pge/draw.hpp"
#include <SDL2/SDL.h>
#include <cstdio>
#include <iostream>

#include <curl/curl.h>
#include <stdio.h>
#include <vector>

int main(int argc, char** argv)
{
    pge::init();
    pge::state::push(pge::es_intro);
    pge::handle_statecalls();

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
