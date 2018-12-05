// Viewport managment, calculating drawing scale and offset, primitive drawing.
#ifndef PGE_DRAW_HPP
#define PGE_DRAW_HPP

#include "pge/constants.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>
#include <stdint.h>
namespace pge {

uint32_t get_color();
}

namespace pge::draw {
SDL_Color get_pixel(Sint16 x, Sint16 y);
void pixel(float _x, float _y);
void line(float _x, float _y, float _xx, float _yy);
void rect(float _x, float _y, float _w, float _h);
void rectF(float _x, float _y, float _w, float _h);
void rectR(float _x, float _y, float _w, float _h, float r);
void rectRF(float _x, float _y, float _w, float _h, float r);
void circle(float _x, float _y, float r);
void circleF(float _x, float _y, float r);
void sector(float _x, float _y, float ir, float _or, float sa, float ea);
void sectorF(float _x, float _y, float ir, float _or, float sa, float ea);

// color -  0xRRGGBBAA
void pixel(float _x, float _y, SDL_Color _color);
void rect(float _x, float _y, float _w, float _h, SDL_Color _color);
void rectF(float _x, float _y, float _w, float _h, SDL_Color _color);
void line(float _x, float _y, float _xx, float _yy, SDL_Color _color);
void rectR(float _x, float _y, float _w, float _h, float r, SDL_Color _color);
void rectRF(float _x, float _y, float _w, float _h, float r, SDL_Color _color);
void circle(float _x, float _y, float r, SDL_Color _color);
void circleF(float _x, float _y, float r, SDL_Color _color);
void sector(float _x, float _y, float ir, float _or, float sa, float ea, SDL_Color _color);
void sectorF(float _x, float _y, float ir, float _or, float sa, float ea, SDL_Color _color);

float get_scale();
void set_scale(float _scale);
void set_scale_factor(uint32_t _w, uint32_t _h, uint16_t _factor);
void set_scale_detail(uint8_t _detail);
}

namespace pge::viewport {
struct View {
    double x;
    double y;
    double w;
    double h;
};
float get_xmoved(float _x, View* _vp = NULL);
float get_xmoved_parallax(float _x, float _factor, float _sx = 0,
    View* _vp = NULL);
float get_ymoved(float _y, View* _vp = NULL);
float get_ymoved_parallax(float _y, float _factor, float _sy = 0,
    View* _vp = NULL);

void set(uint32_t _x, uint32_t _y, uint32_t _w, uint32_t _h,
    View* _vp = NULL);

bool contains(int32_t _x, int32_t _y, View* _vp = NULL);
bool contains(int32_t _x, int32_t _y, uint32_t _w, uint32_t _h, View* _vp = NULL);
}

namespace pge::window {

void get_size(int* width, int* height);
void resize(int width, int height);
void set_resolution(int width, int height);
bool is_fullscreen();
void set_fullscreen(bool var);

int w();
int h();

void create(int width = DEFAULT_WIN_WIDTH,
    int height = DEFAULT_WIN_HEIGHT,
    uint32_t window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
    uint32_t renderer_flags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
void rename(std::string name);
GPU_Target* get_renderer();
void set_renderer(GPU_Target*);
GPU_Target* get_target();
Uint32 get();

void render();
void clear();
}

#endif // DESIGNER_HPP
