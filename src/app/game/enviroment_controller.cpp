#include "enviroment_controller.h"
#include "app/game/controller.h"
#include "pge/pge.hpp"
#include <iostream>
#include <vector>

EController::EController(Controller* _control, unsigned int width, unsigned int height) : map(width, height)
{

	for (int j = 0; j < map.h(); j++)
	{
		for (int i = 0; i < map.w(); i++)
		{
			int t = pge::random::get(0, 7);
			if (j < 16)
				map(i, j) = EBlock{ 0, &tiles, &clips[t] };
			else if(j < 80)
				map(i, j) = EBlock{ 1, &tiles2, &clips[t] };
			else
				map(i, j) = EBlock{ 0, &tiles, &clips[t] };
		}
	}
}
EController::~EController() {}

bool EController::generate_world(int seed, int type)
{
	return false;
}

void EController::draw_enviroment(pge::viewport::View view)
{
	float scale = pge::draw::get_scale();
	float tilesize = 12.f;
	int tilex = view.x / tilesize;
	int tiley = view.y / tilesize;
	int tileh = view.h / tilesize;
	int tilew = view.w / tilesize;
	
	int tilews = SDL_min( tilex + tilew + 2, map.w() );
	int tilehs = SDL_min( tiley + tileh + 2, map.h() );

	for (int j = tiley; j < tilehs ; j++)
	{
		for (int i = tilex; i < tilews; i++)
		{
			map(i, j).tileset->draw(pge::viewport::get_xmoved(i * tilesize, &view) * scale,
									pge::viewport::get_ymoved(j * tilesize, &view) * scale,
									map(i, j).clip, 0, 0.f, scale, scale);
		}
	}
}
