#include "pge/constants.hpp"
#include "pge/core.hpp"
#include "pge/debug.hpp"
#include "pge/draw.hpp"
#include "pge/types.hpp"
#include <iostream>
#include <time.h>
#include <exception>
#include <string>

#include <cmath>
#include <vector>

int main(int argc, char* argv[])
{
    pge::init();
    pge::state::push(pge::es_intro);
    pge::handle_statecalls();

    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    // main game loop

	while (pge::is_running()) {
		pge::handle_allevents();
		pge::update();
		pge::window::clear();
		pge::redraw();
		pge::window::render();
		pge::handle_statecalls();
		pge::delay_frame_difference();
        //tStart = clock();
        //double b = (double)(clock() - tStart) / CLOCKS_PER_SEC;
        //pge::debug::log(std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(a + b));
    }

    // clearing session
	pge::debug::log("closing");

	pge::close();
	pge::debug::log("perfect");

    return 0;
}
