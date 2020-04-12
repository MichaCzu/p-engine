#include "app/game/controller.h"
#include "app/game/entity/component/cAll.h"
#include "entityCreator.h"

Entity* EntityCreator::createHumanoid(pge::Pixel _pos, float Dir, float maxspeed)
{
	Entity* ent = ctrl->create(new Entity(ctrl, _pos));
	ent->cActivity = new CAGround(ent, { maxspeed });
	ent->cRender = new CRHumanoid(ent);
	ent->cControl = new CCBasic(ent, Dir);
	return ent;
}

Entity* EntityCreator::createPFollower(pge::Pixel _pos, float Dir, float maxspeed)
{
	Entity* ent = ctrl->create(new Entity(ctrl, _pos));
	ent->cActivity = new CAGround(ent, { maxspeed });
	ent->cRender = new CRHumanoid(ent);
	ent->cControl = new CCFollower(ent, Dir);
	return ent;
}

Entity* EntityCreator::createTreenoid(pge::Pixel _pos)
{
	Entity* ent = ctrl->create(new Entity(ctrl, _pos));
	ent->cActivity = new CANull(ent, { 0.1 });
	ent->cRender = new CRHumanoid(ent);
	ent->cControl = nullptr;// new CCBasic(ent, 0);
	return ent;
}