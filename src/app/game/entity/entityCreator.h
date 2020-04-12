#ifndef ENTITYCREATOR_HPP
#define ENTITYCREATOR_HPP

#include "app/game/entity/entity.h"
#include "pge/pge.hpp"

class Controller;

class EntityCreator {
public:
	Entity* createHumanoid(pge::Pixel _pos, float Dir = 0.0, float maxspeed = 0.4);
	Entity* createPFollower(pge::Pixel _pos, float Dir = 0.0, float maxspeed = 0.4);
	Entity* createTreenoid(pge::Pixel _pos);

protected:
	friend Controller;
	Controller* ctrl;
	EntityCreator() {}
	//~EntityCreator() {}

	EntityCreator(EntityCreator const&) = delete;
	void operator=(EntityCreator const&) = delete;

	void attachController(Controller* _ctrl) 
	{
		ctrl = _ctrl;
	}

public: 
	static EntityCreator& instance()
	{
		static EntityCreator instance;
		return instance;
	}
};

#endif
