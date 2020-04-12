#pragma once

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

public:
	void setup(EntityState* initalState) {}
	void update(uint16_t diff) {}
};