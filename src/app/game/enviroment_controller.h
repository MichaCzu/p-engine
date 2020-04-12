#ifndef ECONTROLLER_HPP
#define ECONTROLLER_HPP

#include "app/game/event/event.h"
#include "pge/pge.hpp"

class Controller;



struct EBlock
{
	int id = 0;
	pge::image::Image* tileset = nullptr;
	pge::Rect* clip = nullptr;
};

class EController {
	pge::Matrix2D<EBlock> map;
	pge::image::Image tiles = { "../res/spr/env/grassA.png" };
	pge::image::Image tiles2 = { "../res/spr/env/grassB.png" };

	pge::Rect clips[8] =
	{
		{0,48,12,12}, {12,48,12,12}, {24,48,12,12}, {36,48,12,12},
		{0, 60, 12, 12}, {12, 60, 12, 12}, {24, 60, 12, 12}, {36, 60, 12, 12}
	};

protected:
    friend class Controller;

	EController(Controller* _control, unsigned int width, unsigned int height);
	~EController();

	bool generate_world(int seed, int type);
	void draw_enviroment(pge::viewport::View view);
public:

	int map_w() { return map.w(); }
	int map_h() { return map.h(); }
	
};
#endif
