#include "app/state_menu.hpp"
#include "pge/debug.hpp"
#include "pge/pge.hpp"
#include <cmath>
#include <iostream>
#include <stdint.h>

#include "menu/widget.h"
#include "menu/menu.h"

void State_Menu::init()
{
}

void State_Menu::cleanup()
{
}

//WORKS PERFECTLY
void pop_back_utf8(std::string& utf8)
{
    if (utf8.empty())
        return;

    auto cp = utf8.data() + utf8.size();
    while (--cp >= utf8.data() && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {
        if (cp >= utf8.data())
            utf8.resize(cp - utf8.data());
    }
}
//WORKS PERFECTLY

void State_Menu::handle_events(SDL_Event _event)
{
}

void State_Menu::update()
{
	
}

void State_Menu::draw()
{
}
