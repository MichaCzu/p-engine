#ifndef MENU_CLASS
#define MENU_CLASS

#include "app/menu/settings_man.hpp"
#include "pge/pge.hpp"

class MenuClass {
    bool back;

public:
    uint8_t flag = 255;
    virtual void reload() = 0;
    virtual void logic(SDL_Scancode key) = 0;
    uint8_t check(float fun);
    virtual void draw(float fun) = 0;
    void increment(int* var, int range);
    void decrement(int* var, int range);
};

void MenuClass::increment(int* var, int range)
{
    if (*var < range)
        *var = *var + 1;
    else
        *var = 0;
}
void MenuClass::decrement(int* var, int range)
{
    if (*var > 0)
        *var = *var - 1;
    else
        *var = range;
}

uint8_t MenuClass::check(float fun)
{
    uint8_t buff;
    if (fun == 1 || flag > 100) {
        buff = flag;
        flag = 255;
        if (flag <= 100)
            back = true;
    } else
        return 255;
    return buff;
}

#endif
