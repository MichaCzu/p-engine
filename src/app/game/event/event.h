#ifndef EVENTS_H
#define EVENTS_H

#include "pge/pge.hpp"

class Entity;

enum eEventType
{
	evt_attack,
	evt_unspecified
};

class Event {
public:
	eEventType type;
    Event() { type = evt_unspecified; }
};

class EventAttack : Event {
public:
	Entity* sender;
	pge::Pixel source;
	int value;

	EventAttack(int _value) 
	{ 
		type = evt_attack;
		value = _value;
	}
};

#endif
