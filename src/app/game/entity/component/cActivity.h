#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "pge/pge.hpp"

class Entity;
class EntityCreator;
class EntityData;

struct ActivityStats
{
	float maxspeed = 1.0;
};

class CActivity {

protected:
    friend class Entity;
	friend class EntityCreator;
	Entity* src;

	ActivityStats stats;
	pge::Pixel velocity;
	pge::Pixel acceleration;

	bool pushable = false;
	bool mobile = false;

	CActivity(Entity* _src) : CActivity(_src, stats) {}
	CActivity(Entity* _src, ActivityStats stat)
        : src(_src)
    {
		velocity = pge::Pixel::Zero();
		acceleration = pge::Pixel::Zero();
		stats = stat;
    }
    ~CActivity() {}

	virtual void update(EntityData& data, uint16_t diff) = 0;
	virtual void onTick(EntityData& data) = 0;

public:
	virtual void accelerate(pge::Pixel vector) = 0;
	virtual void push(pge::Pixel vector) = 0;
	virtual void push(pge::Pixel source, double val) = 0;

	pge::Pixel getVelocity() { return velocity; }
	void setMaxvel(float val) { stats.maxspeed = val>0 ? val : 0; }
	float getMaxvel() { return stats.maxspeed; }
	bool isPushable() { return pushable; }
};

#endif
