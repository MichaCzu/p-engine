#pragma once

#include "pge/pge.hpp"
#include "app/game/entity/component/cActivity.h"

class CANull : private CActivity {

protected:
	friend class Entity;
	friend class EntityCreator;

	CANull(Entity* _src, ActivityStats _stats)
		: CActivity(_src, _stats)	{	}
	~CANull() {}

	void accelerate(pge::Pixel vector) {}
	void push(pge::Pixel vector) {}
	void push(pge::Pixel source, double val) {}


public:
	void update(EntityData& data, uint16_t diff) {}
	void onTick(EntityData& data) {}
};
