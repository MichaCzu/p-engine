#ifndef WIDGET_HPP
#define WIDGET_HPP
#include "pge/draw.hpp"
#include "pge/image.hpp"
#include "pge/text.hpp"
#include <vector>

enum enMenus {
    mMain = 0,
    mNewGame,
    mLoadGame,
    mOptions,
    mSound,
    mGraphics,
    mControls,
    mCredits,
    mQuit
};

class Menu;

//template

class Widget {
private:
    Menu* menu;
    int h;
    bool lock;

public:
    int get_h() { return h; }
    virtual void event(SDL_Event _ev) = 0;
    virtual void activate() = 0;
    virtual void shift_right() = 0;
    virtual void shift_left() = 0;
    virtual void draw() = 0;
};

class Menu {
    std::vector<Widget> widgets;
    int selected = 0;

public:
    Widget* get_widget;

    bool push_signal();
    void _event(SDL_Event _ev);
    void activate() { widgets[selected].activate(); }
    void shift_right() { widgets[selected].shift_right(); }
    void shift_left() { widgets[selected].shift_left(); }
    void shift_down()
    {
        selected++;
        if (selected > widgets.size())
            selected = 0;
    }
    void shift_up()
    {
        if (selected < 0)
            selected = widgets.size();
    }

    void draw(int x, int y, int w)
    {
    }
};

#endif
