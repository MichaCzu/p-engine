#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "pge/pge.hpp"
#include "app/game/entity/entityData.h"
#include <array>

class Entity;
class EntityCreator;
class EntityData;


class CRender {
protected:
    friend class Entity;
	friend class EntityCreator;
	EntityData renderCopy;
	Entity* src;
	bool isRed = false;

	CRender(Entity* _src)
        : src(_src)
    {
    }
    ~CRender() {}

public:
	virtual void update(EntityData& data, uint16_t diff) = 0;
	virtual void render(pge::viewport::View& viewport) = 0;//
	virtual void render_shadow(pge::viewport::View& viewport) = 0;//

	void set_red()
	{
		isRed = true;
	}
};

struct CRFrame
{
	int time;
	pge::Rect frame;

	CRFrame(int _time, pge::Rect _frame)
	{
		time = _time;
		frame = _frame;
	}
};

class CRAnim
{
	int frameNumber;
	CRFrame* frames = nullptr;

public:
	bool repeat;
	float animTime = 0;
	int selected = 0;
	uint16_t id;

			  CRAnim(uint16_t _id, int _frameNumber, CRFrame* _frames, bool _repeat)
	{
				  changeAnim(_id, _frameNumber, _frames, _repeat);
	}

	void  changeAnim(uint16_t _id, int _frameNumber, CRFrame* _frames, bool _repeat)
	{
		frameNumber = _frameNumber;
		frames = _frames;
		repeat = _repeat;
		id = _id;
		//selected = 0;
	}

	bool update(float timeDiff)
	{
		animTime += timeDiff;
		while(animTime >= (float)frames[selected].time)
		{
			if (selected < frameNumber - 1) {
				animTime -= (float)frames[selected].time;
				selected++;
			}
			else if (repeat) {
				animTime -= (float)frames[selected].time;
				selected = 0;
			}
			else {
				return true;
			}
		}
		return false;
	}

	pge::Rect getFrame()
	{
		return frames[selected].frame;
	}

	int getTime()
	{
		int buff=0;
		for (int i = 0; i < selected; i++)
		{
			buff += frames[i].time;
		}
		return buff + animTime;
	}
};

#endif
