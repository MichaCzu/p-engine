#pragma once

#include "pge/pge.hpp"
#include "app/game/entity/component/cRender.h"
#include "app/game/entity/entity.h"

class CRHumanoid : private CRender {
	pge::image::Image* mainimg;
	pge::image::Image* secimg;
	pge::image::Image* swordimg;
	pge::image::Image* shieldimg;
	bool dirRight = true;
	bool dirDown = true;

	int tim = 0;

	

protected:
    friend class Entity;
	friend class EntityCreator;
	
	CRFrame aIdle[1] = { {5000, pge::Rect(20, 0, 13, 13) } };
	CRFrame aWalk[4] = { {200, pge::Rect(33, 0, 13, 13) }, {200, pge::Rect(33, 13, 13, 13) }, {200, pge::Rect(33, 26, 13, 13) }, {200, pge::Rect(33, 39, 13, 13) } };
	CRFrame aRun[4] =  { {200, pge::Rect(46, 0, 13, 13) }, {200, pge::Rect(46, 13, 13, 13) }, {200, pge::Rect(46, 26, 13, 13) }, {200, pge::Rect(46, 39, 13, 13) } };
	CRFrame aBlink[2] = { {6000, pge::Rect(13, 0, 7, 4) }, {250, pge::Rect(13, 4, 7, 4) } };
	//int xD[2] = { 10,20 };
	CRAnim animator = { 1, 1, aIdle, true };
	CRAnim animeye = { 1, 2, aBlink, true };
	pge::Pixel lookDir;


    CRHumanoid(Entity* _src)
        : CRender(_src)
    {
		lookDir = pge::Pixel((float)pge::random::get(-100, 100) * 0.01f, (float)pge::random::get(-100, 100) * 0.01f).mod(2.5f);
		mainimg = Resourcer::instance().load_image("../res/spr/player/base/base.png");

		bool isKnight = false;

		int rand = pge::random::get(1, 100);
		if (rand < 5) {
			mainimg = Resourcer::instance().load_image("../res/spr/player/base/basegold.png");
			isKnight = true;
		}		
		else if (rand < 50)
			mainimg = Resourcer::instance().load_image("../res/spr/player/base/base01.png");
		else if (rand < 65)
			mainimg = Resourcer::instance().load_image("../res/spr/player/base/base02.png");
		else if (rand < 75)
			mainimg = Resourcer::instance().load_image("../res/spr/player/base/base03.png");
		else if (rand < 82)
			mainimg = Resourcer::instance().load_image("../res/spr/player/base/base04.png");
		else if (rand < 93)
			mainimg = Resourcer::instance().load_image("../res/spr/player/base/base05.png");
		else
		{
			mainimg = Resourcer::instance().load_image("../res/spr/player/base/base.png");
			isKnight = true;
		}

		rand = pge::random::get(1, 100);
		if (isKnight)
			swordimg = Resourcer::instance().load_image("../res/spr/weapons/sword1.png");
		else if (rand<30)
			swordimg = Resourcer::instance().load_image("../res/spr/weapons/sword.png");
		else if (rand<60)
			swordimg = Resourcer::instance().load_image("../res/spr/weapons/empty.png");
		else if (rand < 72)
			swordimg = Resourcer::instance().load_image("../res/spr/weapons/sword1.png");
		else if (rand < 90)
			swordimg = Resourcer::instance().load_image("../res/spr/weapons/katana.png");
		else
			swordimg = Resourcer::instance().load_image("../res/spr/weapons/dagger.png");

		rand = pge::random::get(1, 100);
		if (isKnight)
			shieldimg = Resourcer::instance().load_image("../res/spr/weapons/shield2.png");
		else if (rand < 40)
			shieldimg = Resourcer::instance().load_image("../res/spr/weapons/empty.png");
		else if (rand < 80)
			shieldimg = Resourcer::instance().load_image("../res/spr/weapons/shield.png");
		else
			shieldimg = Resourcer::instance().load_image("../res/spr/weapons/shield1.png");

		animeye.animTime = rand * 20;
    }
    ~CRHumanoid() {}

	void draw_body(pge::viewport::View& viewport, bool down, bool right, float scale);
	void draw_rarm(pge::viewport::View& viewport, bool down, bool right, float scale);
	void draw_larm(pge::viewport::View& viewport, bool down, bool right, float scale);
	void draw_head(pge::viewport::View& viewport, bool down, bool right, float scale);

public:
	void update(EntityData& data, uint16_t diff)
	{
		if (src->getActivity() != nullptr)
		{

			float speed = src->getActivity()->getVelocity().magnitude();

			if (speed > 0.9f)
			{
				if (animator.id != 3)
					animator.changeAnim(3, 4, aRun, true);
				animator.update((float)diff * speed / 1.1);
			}
			else if (speed > .1)
			{
				if (animator.id != 2)
					animator.changeAnim(2, 4, aWalk, true);
				animator.update((float)diff * speed / 0.5);
			}
			else
			{
				if (animator.id != 1)
					animator.changeAnim(1, 1, aIdle, true);
				animator.update(diff);
			}
			animeye.update(diff);

			if (src->getActivity()->getVelocity().magnitude() > 0.01)
				lookDir = (lookDir + src->getActivity()->getVelocity().mod(0.01f * (float)diff)).trim(2.5);
		}

		renderCopy = data;
	}

	void render_shadow(pge::viewport::View& viewport)
	{
		const pge::Rect shadow = { 72,0,6,12 };
		float scale = pge::draw::get_scale();
		mainimg->draw(pge::viewport::get_xmoved(renderCopy.position.x-3, &viewport) * scale,
					pge::viewport::get_ymoved(renderCopy.position.y-6.5, &viewport) * scale, &shadow, 0, 90.0f, scale, scale);


	}

	void render(pge::viewport::View& viewport);


};
