#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "app/game/enviroment_controller.hpp"
#include "app/game/player_controller.hpp"
#include "app/game/tools/light_controller.hpp"
#include "app/game/tools/resource_controller.hpp"
#include "pge/pge.hpp"
#include <SDL2/SDL.h>
#include <vector>

class Entity;
class Player;
class Particle;
class EController;

class Controller;

class Controller {
    uint8_t room;
    uint8_t lives;
    uint8_t points;
    std::vector<Entity*> vCreateCalls;
    std::vector<Entity*> vEntities;
    std::vector<Particle*> vParticles;
    RController Rcontrol;
    PController Pcontrol;
    LController Lcontrol;
    EController Econtrol = EController(&Lcontrol, "plains", "sky0");
    pge::viewport::View viewport;
    float vpx = 0, vpy = 0;
    uint16_t fixedstep_rest;

    float stopmotion_target = 1;
    float stopmotion_factor = 1;

protected:
    friend class State_Game;
    uint16_t fronttexture;
    Controller();
    ~Controller();
    void handle_event(SDL_Event _event);
    void handle_create();
    void handle_destroy();

    void handle_prelogic();
    void handle_collision();
    void handle_fixedstep(); //(100fps)
    void handle_logic();

    void handle_camera();
    void handle_draw_background();
    void handle_draw();
    std::vector<int>* handle_draw_particles();
    void handle_draw_particles_front(std::vector<int>* part);
    void handle_draw_light();
    void handle_draw_gui();

    void adjust_draw();

public:
    //object management
    pge::viewport::View get_vp() { return viewport; }
    RController* res() { return &Rcontrol; };
    EController* env() { return &Econtrol; };
    PController* plr() { return &Pcontrol; };
    bool block_move(float* x, float* y);

    uint8_t destroy(Entity* _id);
    Entity* create(Entity* _entity);

    void create_particles(Particle* particle);
    void set_motion(float target) { stopmotion_target = target; }
};

#endif
