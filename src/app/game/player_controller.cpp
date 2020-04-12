#include "player_controller.h"
#include "app/game/controller.h"
#include "app/game/entity/component/control/cCPlayerInput.h"
#include "pge/pge.hpp"
#include <iostream>
#include <vector>

PController::PController(Controller* _control)
{
    control = _control;
}

PController::~PController()
{
	if (lastEntityControlComponent != nullptr)
		delete lastEntityControlComponent;
}

bool PController::attach_player(Entity* _player)
{
	bool flag = player != nullptr;
	if (_player != nullptr && _player!=player)
	{
		player = _player;
		lastEntityControlComponent = player->cControl;
		player->cControl = new CCPlayerInput(player);
	}
	return flag;
}

bool PController::detach_player()
{
	bool flag = player != nullptr;
	if (flag)
	{
		player->cControl = lastEntityControlComponent;
		player = nullptr;
	}
	return flag;
}

void PController::handle_event(SDL_Event& e)
{

}

void PController::draw_gui(pge::viewport::View& vp)
{
}

pge::Pixel PController::get_pos() 
{
	if (player != nullptr)
		return player->get_pos();
	else
		return lastpos;
}
