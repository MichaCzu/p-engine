#include "app/game/controller.h"
#include "app/game/entity/entity.h"
#include "app/game/player_controller.h"
#include "app/game/enviroment_controller.h"
#include "app/game/entity/entityCreator.h"
#include "pge/pge.hpp"

#include <time.h>
#include <vector>
#include <string>
//object management

Controller::Controller()
	: Pcontrol(this), Econtrol(this, 160, 100)
{
	threadMutex = SDL_CreateMutex();
	EntityCreator::instance().attachController(this);
	pge::random::set_seed(0);
	pge::draw::set_scale_detail(2);
	pge::draw::set_scale(3.f);
	isRunning = true;
	logicThread = SDL_CreateThread(threadLogicController, "logic thread", this);
}

Controller::~Controller()
{
	isRunning = false;
	SDL_WaitThread(logicThread, &threadReturnVal);
	SDL_DestroyMutex(threadMutex);

	for (int i = 0; i < vEntities.size(); i++) {
		delete vEntities[i];
	}
	vEntities.erase(vEntities.begin(), vEntities.end());
	Resourcer::instance().free_everything();
}

void Controller::EntitySort(int n)
{
    int i, j;
    Entity* key;

    for(i = 1; i < n; i++) {
        key = vEntities[i];
        j = i - 1;

        while (j >= 0 && (vEntities[j]->get_pos().y > key->get_pos().y)) {
            vEntities[j + 1] = vEntities[j];
            j = j - 1;
        }
        vEntities[j + 1] = key;
    }
}

void Controller::handle_event(SDL_Event _event)
{
	Pcontrol.handle_event(_event);
	//
	//TODO: probably more code
	//
	if (_event.type == SDL_KEYDOWN)
	{
		switch (_event.key.keysym.scancode)
		{
		case SDL_Scancode::SDL_SCANCODE_1:
			if(pge::draw::get_scale()<8)
				pge::draw::set_scale(pge::draw::get_scale() + 0.5);
			break;
		case SDL_Scancode::SDL_SCANCODE_2:
			if (pge::draw::get_scale()>1)
				pge::draw::set_scale(pge::draw::get_scale() - 0.5);
			break;
		case SDL_Scancode::SDL_SCANCODE_3:
			pge::draw::set_scale(3.f);
			break;
		case SDL_Scancode::SDL_SCANCODE_4:
			pge::draw::set_scale(4.f);
			break;
		case SDL_Scancode::SDL_SCANCODE_5:
			plr()->detach_player();
			plr()->attach_player(vEntities[pge::random::get(0,vEntities.size()-1)]);
			break;
		}
	}
}

void Controller::handle_create()
{
    for (int i = 0; i < vCreateCalls.size(); i++) {
        vEntities.push_back(vCreateCalls[i]);
    }
    vCreateCalls.clear();
}

void Controller::handle_destroy()
{
    for (int i = 0; i < vEntities.size(); i++)
        if (vEntities[i]->flagDestroy) {
            delete vEntities[i];
            vEntities.erase(vEntities.begin() + i);
            i--;
        }
}

void Controller::handle_prelogic(uint32_t time)
{
	//auto time = pge::get_time_difference();
    for (int i = 0; i < vEntities.size(); i++)
        vEntities[i]->prelogic(time * stopmotion_factor);
}

void Controller::handle_fixedstep(uint32_t time)
{
    uint16_t steptime;
    uint16_t steps;

    steptime = time * stopmotion_factor + fixedstep_rest;
    steps = steptime / 20;
    fixedstep_rest = steptime % 20;

    for (; steps > 0; steps--) {
        for (int j = 0; j < vEntities.size(); j++)
            vEntities[j]->fixedstep();
        //for (int j = 0; j < vParticles.size(); j++)
		//	vParticles[j]->fixedlogic();
    }
}

void Controller::handle_setneighbours()
{
	for (int i = 0; i < vEntities.size(); i++)
	{
		vEntities[i]->neighbours.clear();
	}

	const int dist = 80;
	for (int i = 0; i < vEntities.size() - 1; i++) 
	{
		pge::Pixel pos = vEntities[i]->get_pos();
		for (int j = i+1; j < vEntities.size(); j++)
		{
			if (vEntities[j]->get_pos().y < pos.y + dist)
			{
				if (vEntities[j]->get_pos().x < pos.x + dist && vEntities[j]->get_pos().x > pos.x - dist)
				{
					vEntities[j]->neighbours.push_back(vEntities[i]);
					vEntities[i]->neighbours.push_back(vEntities[j]);
				}
			}
			else
				break;
		}
	}
}

void Controller::handle_logic(uint32_t time)
{
    for (int i = 0; i < vEntities.size(); i++)
        vEntities[i]->logic(time * stopmotion_factor);
}

void Controller::adjust_draw()
{
	Resourcer::instance().handle_loading();
}

void Controller::handle_predraw(uint32_t time)
{
    float xx, yy, ty, scale = pge::draw::get_scale();
    pge::Pixel ppos = Pcontrol.get_pos();

    int flagw, flagh;
    pge::window::get_size(&flagw, &flagh);

    viewport.w = flagw / scale;
    viewport.h = flagh / scale;
    viewport.x = ppos.x - viewport.w / 2.f;
    viewport.y = ppos.y - viewport.h / 2.f;

    if (viewport.x < 0)
        viewport.x = 0;
    if (viewport.x > (float)Econtrol.map_w() * 12.f - viewport.w)
        viewport.x = (float)Econtrol.map_w() * 12.f - viewport.w;
    if (viewport.y < 0)
        viewport.y = 0;
    if (viewport.y > (float)Econtrol.map_h() * 12.f - viewport.h)
        viewport.y = (float)Econtrol.map_h() * 12.f - viewport.h;
	viewportBox = pge::Rect{ viewport.x - viewport.w * 0.1f, viewport.y - viewport.w * 0.1f, viewport.x + viewport.w * 1.1f, viewport.y + viewport.h * 1.1f };

	for (int i = 0; i < vEntities.size(); i++) {
		if (isInBox(vEntities[i]->get_pos(), viewportBox))
			vEntities[i]->prep_draw(time * stopmotion_factor);
	}
}

void Controller::handle_draw_background()
{
    // ???
}

void Controller::handle_draw()
{
	static pge::Pixel oldvp = { 0,0 };
	if (oldvp != viewport.size())
	{
		int nw, nh;
		pge::window::get_size(&nw, &nh);
		shadowsPlane.query(nw, nh);
		oldvp = viewport.size();
	}
    //drawing floor

	Econtrol.draw_enviroment(vp());
    //pge::draw::circleF(250 - viewport.x, 250 - viewport.y, 500, SDL_Color{ 100, 100, 100, 255 });
	

	//drawing shadows
	pge::image::set_target(shadowsPlane.get_id());
	pge::window::clear();
	for (int i = 0; i < vEntities.size(); i++) {
		if (isInBox(vEntities[i]->get_pos(), viewportBox))
			vEntities[i]->draw_shadow(viewport);
		//pge::debug::log(std::to_string(i) + " " + std::to_string(vEntities[i]->get_pos().y));
	}
	pge::image::reset_target();
	shadowsPlane.set_colormod({ 255, 255, 255, 55 });
	shadowsPlane.draw(0, 0);

	//Drawing entities
    for (int i = 0; i < vEntities.size(); i++) {
		if (isInBox(vEntities[i]->get_pos(), viewportBox))
	        vEntities[i]->draw(viewport);
        //pge::debug::log(std::to_string(i) + " " + std::to_string(vEntities[i]->get_pos().y));


    }
}

std::vector<int>* Controller::handle_draw_particles()
{
    /*
        std::vector<int>* output = new std::vector<int>;
        for (int i = 0; i < vParticles.size(); i++) {
            if (!vParticles[i]->isfront)
                vParticles[i]->draw(viewport);
            else
                output->push_back(i);
        }
        return output;
    */
    return nullptr;
}

void Controller::handle_draw_particles_front(std::vector<int>* part)
{
    /*
    for (int i = 0; i < part->size(); i++)
        vParticles[(*part)[i]]->draw(viewport);
    part->clear();
    delete part;
    */
}

void Controller::handle_draw_light()
{
    //static uint32_t nexttime = 0;
    //if (nexttime < pge::get_time_passed()) {
    //    nexttime = pge::get_time_passed() + 1000 / 20;
    //Lcontrol.update();
    //}
    //pge::image::set_target(fronttexture);
    //Lcontrol.draw();
}

void Controller::handle_draw_gui()
{
	auto fps = pge::get_fps();
	std::string fpss = std::to_string(fps);
	std::wstring fpsw;
	fpsw.assign(fpss.begin(),fpss.end());
	font1.draw(0, 0, fpsw);

	fpss = std::to_string(threadFps);
	fpsw.assign(fpss.begin(), fpss.end());
	font1.draw(0, 16, fpsw);

	fpss = std::to_string(vEntities.size());
	fpsw.assign(fpss.begin(), fpss.end());
	font1.draw(0, 32, fpsw);
}

//object managementfloor
uint8_t Controller::destroy(Entity* _entity)
{
    for (int i = 0; i < vEntities.size(); i++) {
        if (vEntities[i] == _entity) {
            vEntities[i]->flagDestroy = true;
            return 0;
        }
    }

    return 1;
}
Entity* Controller::create(Entity* _entity)
{
    vEntities.push_back(_entity);
    return _entity;
}



//bad
/*
bool Controller::get_collision(pge::Pixel a, pge::Pixel b)
{
	if (b.x > a.x) { double g = a.x; a.x = b.x; b.x = g; }
	if (b.y > a.y) { double g = a.y; a.y = b.y; b.y = g; }

	std::list<Entity*> store;
	for (int i = 0; i < vEntities.size(); i++)
	{
		if (!vEntities[i]->entityData.collidable)
			continue;

		pge::Pixel e = vEntities[i]->get_pos();
		if (e.x >= a.x && e.x <= b.x && e.y > a.y && e.y <= b.y)
		{
			store.push_back(vEntities[i]);
		}
	}
	return store;
}

std::list<Entity*> Controller::get_collision(pge::Pixel a, int w, int h)
{
	pge::Pixel b = pge::Pixel(a.x + w, a.y + h);
	return get_collision(a, b);
}
std::list<Entity*> Controller::get_collision(pge::Pixel source, float distance)
{
	std::list<Entity*> store;
	pge::Pixel test = pge::Pixel(1, 1);

	for (int i = 0; i < vEntities.size(); i++)
	{
		if (!vEntities[i]->entityData.collidable)
			continue;

		pge::Pixel e = vEntities[i]->get_pos();
		if (source.distanceTo(e) - vEntities[i]->entityData.radius <= distance)
		{
			store.push_back(vEntities[i]);
		}
	}
	return store;
}

std::list<Entity*> Controller::get_collision(pge::Pixel source, float distance, float angle, float maxangle)
{
	std::list<Entity*> store;
	pge::Pixel test = pge::Pixel(1, 1);

	for (int i = 0; i < vEntities.size(); i++)
	{
		if (!vEntities[i]->entityData.collidable)
			continue;

		pge::Pixel e = vEntities[i]->get_pos();
		if ((source.distanceTo(e) - vEntities[i]->entityData.radius <= 0) || (source.distanceTo(e) - vEntities[i]->entityData.radius <= distance && source.angle(e) - angle < maxangle))
		{
			store.push_back(vEntities[i]);
		}
	}
	return store;
}
*/