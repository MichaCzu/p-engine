#pragma once

#include <vector>

enum EntityStateEnum
{
	Idle,
	Walk,
	Run,
	Attack1,
	Attack2,
	Attack3,
	Attack4,
};

class EntityState
{
	EntityStateEnum type;

public:
	void init() {}
	void finish() {}
	void update(uint16_t diff) {}

	EntityStateEnum getType() { return type; }
};

class EntityStateMachine
{
	std::vector<EntityState> states;
public:
	void setup(EntityState initialState) 
	{
		states.push_back(initialState);
	}
	void update(uint16_t diff) 
	{
		if(!states.empty())
			states.back().update(diff);
	}
};