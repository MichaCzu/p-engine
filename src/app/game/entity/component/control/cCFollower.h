#pragma once

#include "pge/pge.hpp"
#include "app/game/entity/component/cControl.h"
#include "app/game/entity/component/cActivity.h"


class CCFollower : private CControl {

protected:
    friend class Entity;
	friend class Controller;
	friend class EntityCreator;

	float dir = 0.0;
	bool followPlayer;

	CCFollower(Entity* _src, float initialDir=0)
        : CControl(_src)
    {
		dir = initialDir;
    }
    ~CCFollower() {}

public:
	void onTick(EntityData& data);
	void update(EntityData& data, uint16_t diff);
};
