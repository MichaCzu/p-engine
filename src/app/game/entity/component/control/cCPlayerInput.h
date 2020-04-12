#pragma once

#include "app/game/entity/entity.h"
#include "app/game/entity/component/cControl.h"
#include "app/game/entity/component/cActivity.h"
#include "pge/pge.hpp"

class PController;

class CCPlayerInput : private CControl {

protected:
	friend class PController;
	friend class Entity;
	friend class Controller;

	CCPlayerInput(Entity* _src)
		: CControl(_src)
	{
	}
	~CCPlayerInput () {}

public:
	void onTick(EntityData& data)
	{
		if (pge::input::get_state(en_inputbtn::ec_slowwalk))
		{
			src->getActivity()->setMaxvel(1.2f);
		}
		else
			src->getActivity()->setMaxvel(0.6f);

		if (pge::input::get_state(en_inputbtn::ec_interact))
		{
			pge::Pixel movevector = (data.position - src->getControl()->get_mousepos()).deadzone(100).trim(500);
			src->getActivity()->accelerate( movevector * 0.002f);
			src->getActivity()->setMaxvel( movevector.magnitude() * 0.006f );
		}
		else if (pge::input::get_state(ea_movex) != 0 || pge::input::get_state(ea_movey) != 0)
		{
			pge::Pixel movevector = pge::Pixel(pge::input::get_state(ea_movex), pge::input::get_state(ea_movey)).deadzone(0.25).trim(1);
			src->getActivity()->accelerate(movevector);
		}
		else 
			src->getActivity()->accelerate(pge::Pixel::Zero());
	}
	void update(EntityData& data, uint16_t diff)
	{

	}
};
