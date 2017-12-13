#ifndef PCONTROLLER_HPP
#define PCONTROLLER_HPP

#include "app/game/player/inventory.hpp"
#include "pge/pge.hpp"

class Controller;
class Player;

class PController {
    uint16_t gui;
    Player* player;
    Controller* control;
    float smooth_life;
    bool inventoryOpen = false;
    PInventory inventory;

protected:
    friend class Controller;

    PController(Controller* _control, float _x, float _y);
    ~PController();

    Player* get_player() { return player; }
    void handle_event(SDL_Event& e);
    void draw_gui(pge::viewport::View& vp);

public:
    void signal_death();
    void get_pos(float* x, float* y);
    void use_item(Item* _item);
    bool check_item_requirements(Item* _item);
    //void pickup_item();
};

#endif
