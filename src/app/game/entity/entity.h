#ifndef ENTITY_H
#define ENTITY_H

/*
/ Entity types:
/ p - Player
/ f - Friendly
/ e - Enemy
/ n - Npc
/ v - enViroment
/ o - Other
/ i - item
*/
#include "app/game/entity/component/cControl.h"
#include "app/game/entity/component/cActivity.h"
#include "app/game/entity/component/cRender.h"
#include "app/game/entity/component/cStats.h"
#include "app/game/resourcer.h"
#include "app/game/event/event.h"
#include "pge/draw.hpp"
#include "pge/types.hpp"
#include "entityData.h"
#include "stateMachine.h"
#include <iostream>
#include <vector>

class Controller;
class PController;
class EntityCreator;

class Entity {

protected:
    friend class Controller;
	friend class EntityCreator;
	friend class PController;

	CControl* cControl = nullptr;	    //controlling input or script
    CActivity* cActivity = nullptr; //movement
    CRender* cRender = nullptr;   //drawing
    CStats* cStats = nullptr;     //entity characteristics, inventory and status
	EntityStateMachine statemachine;

    Controller* control;
	EntityData entityData;
	std::vector<Entity*> neighbours;

	bool flagDestroy = 0;
	bool flagSave = 0; //flags if entity should be saved
	char type = 'a';

	
	virtual ~Entity()
    {
		if (cControl != nullptr)
			delete cControl;
		if (cActivity != nullptr)
			delete cActivity;
		if (cRender != nullptr)
			delete cRender;
		if (cStats != nullptr)
			delete cStats;
    }

    void prelogic(uint16_t diff) 
	{
		if (cControl != nullptr)
			cControl->update(entityData, diff);
		if (cStats != nullptr)
			cStats->update(entityData, diff);
		statemachine.update(diff);
	}

    void fixedstep() 
	{
		if (cStats != nullptr)
			cStats->onTick(entityData);
		if (cControl != nullptr)
			cControl->onTick(entityData);
		if (cActivity != nullptr)
			cActivity->onTick(entityData);
	}

    void logic(uint16_t diff) 
	{
		if (cActivity != nullptr)
			cActivity->update( entityData ,diff);
	}

	void prep_draw(uint16_t diff)
	{
		if (cRender != nullptr)
			cRender->update(entityData, diff);
	}

    void draw(pge::viewport::View& vp) 
	{
		if (cRender != nullptr)
			cRender->render(vp);
	}
	void draw_shadow(pge::viewport::View& vp)
	{
		if (cRender != nullptr)
			cRender->render_shadow(vp);
	}

public:
    Entity(Controller* _control, pge::Pixel _pos) //breed
    {
        control = _control;
        entityData.position = _pos;
    }

	CControl* getInput() { return cControl; }
	CActivity* getActivity() { return cActivity; }
	CRender* getRender() { return cRender;  }
	CStats* getStats() { return cStats; }    
	Controller* getControl() { return control; }
	std::vector<Entity*> getNeighbours() { return neighbours; }
	//void receive();

	pge::Pixel get_pos() { return entityData.position; }
	bool is_collidable() { return entityData.collidable; }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	virtual void destroy(uint16_t value) { this->flagDestroy = true; }
};

#endif
