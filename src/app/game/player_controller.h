 #ifndef PCONTROLLER_HPP
#define PCONTROLLER_HPP

#include "app/game/event/event.h"
#include "pge/pge.hpp"

class Controller;
class CControl;

class PController {
    uint16_t gui; //?
    Entity* player = nullptr;
	CControl* lastEntityControlComponent;
	pge::Pixel lastpos = pge::Pixel(0, 0);
    Controller* control;


protected:
    friend class Controller;

    PController(Controller* _control);
    ~PController();

	bool attach_player(Entity* _player);
	bool detach_player();

    void handle_event(SDL_Event& e);
    void draw_gui(pge::viewport::View& vp);

public:
    Event onNotify(Entity* s, Event e);

	pge::Pixel get_pos();
	Entity* get_player() { return player; }
	bool is_player(Entity* tested) { return (player == tested); }
};

#endif
