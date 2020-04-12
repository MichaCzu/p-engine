#pragma once

#include "cCFollower.h"
#include "app/game/controller.h"
#include "app/game/player_controller.h"


	void CCFollower::onTick(EntityData& data)
	{
		//pge::Pixel playerPos = src->getControl()->plr()->get_pos();
		//if (data.position.distanceTo(playerPos) > 104)
		//{
		//	return;
		//}

		pge::Pixel movevector = pge::Pixel::Zero();
		//pge::Pixel posVector = src->getControl()->plr()->get_pos() - data.position;

		std::vector<Entity*> neigh = src->getNeighbours();
		for (int i = 0; i < neigh.size(); i++)
		{
			pge::Pixel posVector = data.position - neigh[i]->get_pos();
			if (posVector.magnitude() < 10)
			{
				movevector = posVector.mod(1.f);
			}
		}
		
		movevector = (movevector + pge::Pixel(SDL_sinf(dir), SDL_cosf(dir))).trim(1.f);

		src->getActivity()->accelerate(movevector);
		dir += 0.01;
	}

	void CCFollower::update(EntityData& data, uint16_t diff)
	{

	}
