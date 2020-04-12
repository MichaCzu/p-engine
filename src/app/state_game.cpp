#include "app/state_game.hpp"
#include "app/game/controller.h"
#include "pge/core.hpp"
#include "pge/pge.hpp"
#include <iostream>
#include <stdint.h>
#include "game/resourcer.h"
#include "game/entity/entityCreator.h"
#include "game/resourcer.h"

void State_Game::init()
{
    ctrl = new Controller();
}

void State_Game::cleanup()
{
    delete ctrl;
}

void State_Game::handle_events(SDL_Event _event)
{
	ctrl->handle_event(_event);
}

void State_Game::update()
{
	//int time_diffrence = pge::get_time_difference();

	//ctrl->handle_destroy();
	//ctrl->handle_create();

	//ctrl->EntitySort(ctrl->vEntities.size());

	//ctrl->handle_prelogic(time_diffrence);
	//ctrl->handle_fixedstep(time_diffrence);
	//ctrl->handle_logic(time_diffrence);
}

void State_Game::draw()
{
	ctrl->adjust_draw();

	SDL_LockMutex(ctrl->get_mutex());
	ctrl->handle_predraw(pge::get_time_difference());
	SDL_UnlockMutex(ctrl->get_mutex());

	ctrl->handle_draw_background();
	ctrl->handle_draw();

    ctrl->handle_draw_gui();
}