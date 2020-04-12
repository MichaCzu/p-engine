#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "app/game/event/event.h"
#include "app/game/player_controller.h"
#include "app/game/enviroment_controller.h"
#include "app/game/entity/entityCreator.h"
#include "pge/pge.hpp"
#include <SDL2/SDL.h>
#include "SDL2/SDL_thread.h"
#include <vector>
#include <list>

class Entity;
class Player;
class Particle;

class Controller {
    std::vector<Entity*> vCreateCalls;
    std::vector<Entity*> vEntities;
    PController Pcontrol;
	EController Econtrol;

    pge::viewport::View viewport;
	pge::Rect viewportBox;
    float vpx = 0, vpy = 0;
    uint16_t fixedstep_rest;
	pge::text::Font font1{"../res/font/unifont.ttf", 15};
	pge::image::Image iNoise{ "../res/noise.png" };

    float stopmotion_target = 1;
    float stopmotion_factor = 1;
    void EntitySort(int n);

	SDL_Thread* logicThread;
	int threadReturnVal;
	bool isRunning = false;
	pge::image::Image* bg;
	int threadFps = 1000;
	
	SDL_mutex* threadMutex;
	SDL_mutex* get_mutex() { return threadMutex; }

	pge::image::Image shadowsPlane = { 640, 480 };

	static int threadLogicController(void* ptr)
	{
		Controller* ctrl = (Controller*)ptr;
		uint32_t threadCurrentTime = pge::get_time_passed();
		float defaultFps = SDL_max(pge::get_defaultFps(), 1);
		float rps = 0.f;
		if (defaultFps > 1)
		 rps = pge::get_defaultRps();

		Entity* player = EntityCreator::instance().createHumanoid(pge::Pixel{ 1000,500 });
		ctrl->Pcontrol.attach_player(player);

		float sum = 0;
		int nextTick = 0;
		uint32_t time_diffrence;

		const uint8_t _maxSamples = 255;
		float _tickSum = 0, _diffTick[_maxSamples] = { 0 };
		uint8_t selector = 0;

		pge::debug::log("thread initiated..");
		while (ctrl->isRunning)
		{
			time_diffrence = pge::get_time_passed() - threadCurrentTime;
			threadCurrentTime = pge::get_time_passed();
			time_diffrence = SDL_min(time_diffrence, 200);

			SDL_LockMutex(ctrl->get_mutex());

			//
			{ //not important
				static int timed = -4000;
				static int i = 0;

				static int j = 10;

				if (i < 199)
				{
					timed += time_diffrence;

					while (timed > 10 && i < 199)
					{
						/*
						if (i+1 >= j)
						{
							pge::debug::log(std::to_string(ctrl->vEntities.size()) + ";" + std::to_string(pge::get_fps()) + ";" + std::to_string(ctrl->threadFps));
							j += 10;
						}*/
						//pge::debug::log(std::to_string(ctrl->vEntities.size()) + ";" + std::to_string(pge::get_fps()));

						pge::Pixel location = pge::Pixel{ (float)pge::random::get(20,ctrl->Econtrol.map_w() * 12 - 20), (float)pge::random::get(20,ctrl->Econtrol.map_h() * 12 - 20) };
						int rand = pge::random::get(0, 100);
						
						if (rand > 50)
							EntityCreator::instance().createHumanoid(location, i * 0.06f, 0.4);
						else if (rand > 40)
							EntityCreator::instance().createHumanoid(location, i * 0.06f, 1);
						else if (rand > 20)
							EntityCreator::instance().createHumanoid(location, i * 0.06f, 0.2);
						else
							EntityCreator::instance().createTreenoid(pge::Pixel(location));
						timed -= 10;
						i++;
					}
					//Pcontrol.draw_gui(viewport);
				}

				
			}
			//

			ctrl->handle_destroy();
			ctrl->handle_create();

			ctrl->EntitySort(ctrl->vEntities.size());
			ctrl->handle_setneighbours();

			ctrl->handle_prelogic(time_diffrence);
			ctrl->handle_fixedstep(time_diffrence);
			ctrl->handle_logic(time_diffrence);

			SDL_UnlockMutex(ctrl->get_mutex());

			while (nextTick > SDL_GetTicks()) {
				if (nextTick - SDL_GetTicks() > 1)
					SDL_Delay(1);
			}

			uint32_t ticks = SDL_GetTicks();
			sum += rps;

			if (sum >= 1) {
				nextTick = ticks + defaultFps + 1;
				sum--;
			}
			else
				nextTick = ticks + defaultFps;
			SDL_Delay(1);

			_tickSum -= _diffTick[selector];
			_diffTick[selector] = time_diffrence;
			_tickSum += time_diffrence;
			selector++;
			if (selector >= _maxSamples)
				selector = 0;
			ctrl->threadFps = SDL_ceil(1000.f / (SDL_max((float)_tickSum, 0.0005f) / (float)_maxSamples));
		}
		pge::debug::log("thread closing..");
		return 0;
	}

    //************
    //major observers
    //observer list/vector
    //observer class vs observer function
    //************

protected:
    friend class State_Game;
    Controller();
    ~Controller();
    void handle_event(SDL_Event _event);
    void handle_create();
    void handle_destroy();

    void handle_prelogic(uint32_t timeDiff);
    void handle_fixedstep(uint32_t timeDiff); //(50tps)
	void handle_setneighbours();

    void handle_logic(uint32_t timeDiff);

    
	void handle_predraw(uint32_t timeDiff);
    void handle_draw_background();
    void handle_draw();
    std::vector<int>* handle_draw_particles();
    void handle_draw_particles_front(std::vector<int>* part);
    void handle_draw_light();
    void handle_draw_gui();

    void adjust_draw();

public:
    //object management
	/*
	static bool get_collision(Entity* src, pge::Pixel a, pge::Pixel b);
	static bool get_collision(Entity* src, pge::Pixel a, int w, int h);
	static bool get_collision(Entity* src, pge::Pixel source, float distance);
	static bool get_collision(Entity* src, pge::Pixel source, float distance, float angle, float maxrange);
	*/

    pge::viewport::View vp() { return viewport; }
	pge::Pixel get_mousepos() { return pge::Pixel(viewport.x, viewport.y) + pge::mouse::state(); }
    PController* plr() { return &Pcontrol; };

    bool subscribe(Entity* e);
    bool unsubscribe(Entity* e);
    void notify(Entity* src, Event e);

    uint8_t destroy(Entity* _id);
    Entity* create(Entity* _entity);

    //void create_particles(Particle* particle);
    void set_stopmotion(float target) { stopmotion_target = target; }
	static bool isInBox(pge::Pixel pos, pge::Rect box) { return pos.x > box.x && pos.y > box.y && pos.x < box.w && pos.y < box.h; }
	bool isInFrame(pge::Pixel pos) { return isInBox(pos, viewportBox); }
	pge::text::Font* getFont() { return &font1; }
};

#endif
