#ifndef STATSCOMPONENT_H
#define STATSCOMPONENT_H

#include "pge/pge.hpp"
#include "app/game/entity/component/stats/Buff.h"
#include "app/game/entity/component/stats/Attribute.h"

class Entity;
class EntityCreator;
class EntityData;

class CStats {
    float health;
    float healthghost;
    float energy;
    float energyghost;

	Stat strength;
	Stat agility;
	Stat inteligence;
	Stat vitality;
    
	//Regeneracja zycia powinna wynikac z buffów, ew u¿ycia przedmiotu

protected:
    friend class Entity;
	friend class EntityCreator;
	Entity* src;

	CStats(Entity* _src)
        : src(_src)
    {
    }
    ~CStats() {}

	void addBuff(Buff a) {}
	void removeBuff(Buff a) {}
	void removeBuffs() {}

public:
	void update(EntityData& data, uint16_t diff) {}
	void onTick(EntityData& data) {}
};

#endif
