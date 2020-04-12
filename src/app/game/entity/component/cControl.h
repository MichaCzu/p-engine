#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "pge/pge.hpp"

class Entity;
class EntityCreator;
class PController;
class EntityData;

class CControl {
    

protected:
    friend class Entity;
	friend class EntityCreator;
	friend class PController;
	Entity* src;

	CControl(Entity* _src)
        : src(_src)
    {
    }
    ~CControl() {}

public:
	virtual void onTick(EntityData& data) = 0;
	virtual void update(EntityData& data, uint16_t diff) = 0; 
};

#endif
