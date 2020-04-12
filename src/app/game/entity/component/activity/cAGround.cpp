#include "cAGround.h"
#include "app/game/entity/entity.h"
#include "app/game/controller.h"

void CAGround::accelerate(pge::Pixel vector)
{
	//velocity = velocity + vector;
	acceleration = vector;
}

void CAGround::push(pge::Pixel vector)
{
	velocity = velocity + vector;
}

void CAGround::push(pge::Pixel source, double val)
{
	velocity = velocity + (src->get_pos() - source).mod(val);
}

void CAGround::update(EntityData& data, uint16_t diff)
{
	pge::Pixel acceldiff = (velocity + acceleration).trim(stats.maxspeed) - velocity;

	//if (src->getControl()->)
	data.position = data.position + ((velocity * (float)diff) * 0.05);
	data.position = data.position + (acceldiff * 0.5f * (float)diff * (float)diff * 0.0025);

	velocity = (velocity + acceldiff * 0.01f * (float)diff);

	if (velocity.magnitude() < 0.001 * (float)diff)
		velocity = pge::Pixel::Zero();
	else if (acceleration == pge::Pixel::Zero())
		velocity = velocity - velocity.mod(velocity.magnitude() < 0.05 ? velocity.magnitude() : 0.05) * 0.05 * (float)diff;
}
	

void CAGround::onTick(EntityData& data)
{

	//data.position += pge::Pixel(1, 0); //velocity;
}
