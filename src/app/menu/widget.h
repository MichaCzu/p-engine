#ifndef WIDGET_H
#define WIDGET_H
#include "pge/draw.hpp"
#include "pge/image.hpp"
#include "pge/text.hpp"
#include <vector>

class Menu;

class Widget {
private:
    Menu* menu;
	float x, y;
    int w, h;
    bool lock;

public:
    int get_h() { return h; }
    virtual void onevent(SDL_Event _ev) = 0;
};

#endif
