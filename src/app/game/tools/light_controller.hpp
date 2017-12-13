#ifndef LCONTROLLER_HPP
#define LCONTROLLER_HPP

#include "pge/pge.hpp"
#include <vector>
//
struct LightSource {
    uint16_t x;
    uint16_t y;
    float intensity;

    LightSource(uint16_t _x, uint16_t _y, float _level)
    {
        x = _x;
        y = _y;
        intensity = _level;
    }
};

class Controller;

class LController {
    std::vector<LightSource> vLights;
    uint16_t lighttexture;
    uint16_t lighttexturef;
    uint16_t colormaptexture;
    Controller* control;
    pge::viewport::View* viewport;
    bool switch1 = true;
    bool switch2 = true;
    SDL_Color colormap[51];
    uint8_t skylight = 50;

    float offset_x;
    float offset_y;

    int intensity_test(int x0, int y0, int x1, int y1, float inten = 50);
    int intensity_test(int x0, int y0);
    bool load_colormap(uint8_t val);
    SDL_Color get_color(uint8_t inten);

public:
    SDL_Color get_skycolor() { return get_color(skylight); }
    uint8_t get_skyval() { return skylight; }

protected:
    friend class Controller;
    LController(Controller* _ctrl, pge::viewport::View* _viewport)
    {
        control = _ctrl;
        viewport = _viewport;
        colormaptexture = pge::image::load("res/spr/lightmap.png");
        pge::image::load_target(colormaptexture);
        load_colormap(0);
        lighttexture = pge::image::create(4, 4);
        lighttexturef = pge::image::create(4, 4);

        vLights.push_back(LightSource(0, 0, 60));
        vLights.push_back(LightSource(0, 0, 0));
    }

    void reset();
    void update();
    void draw();

    void switch1_()
    {
        switch1 = switch1 ? false : true;
    }
    void switch2_()
    {
        switch2 = switch2 ? false : true;
        for (int k = vLights.size() - 1; k >= 2; k--)
            vLights.pop_back();
    }

    void bring_light(bool a)
    {
        if (a)
            vLights[1].intensity++;
        else
            vLights[1].intensity--;
    }

    void place_light(int x, int y, int intensity)
    {
        vLights.push_back(LightSource(x, y, intensity));
    }

    uint8_t get_light(int id)
    {
        return vLights[id].intensity;
    }
    uint16_t get_lightx(int id)
    {
        return vLights[id].x;
    }
    uint16_t get_lighty(int id)
    {
        return vLights[id].y;
    }
};

#endif //WORLDGEN
