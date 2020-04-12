#ifndef BUFF_H
#define BUFF_H

#include "pge/pge.hpp"

class Action {
	bool finished = false;
	uint16_t step = 0;
	
	void update(Entity* e, Controller* c) {}
};

#endif
