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
void pixel(int32_t _x, int32_t _y);
void rect(int32_t _x, int32_t _y, int32_t _w, int32_t _h);
void rect_fill(int32_t _x, int32_t _y, int32_t _w, int32_t _h);
void line(int32_t _x, int32_t _y, int32_t _xx, int32_t _yy);

// color -  0xRRGGBBAA
void pixel(int32_t _x, int32_t _y, SDL_Color _color);
void rect(int32_t _x, int32_t _y, int32_t _w, int32_t _h,
    SDL_Color _color);
void rectFill(int32_t _x, int32_t _y, int32_t _w, int32_t _h,
    SDL_Color _color);
void line(int32_t _x, int32_t _y, int32_t _xx, int32_t _yy,
    SDL_Color _color);

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

bool contains_point(int32_t _x, int32_t _y, View* _vp = NULL);
bool contains_rect(int32_t _x, int32_t _y, uint32_t _w, uint32_t _h,
    View* _vp = NULL);
}

namespace pge::window {

void get_size(int* width, int* height);
void set_size(int width, int height);
bool is_fullscreen();
void set_fullscreen(bool var);

void create(int width = DEFAULT_WIN_WIDTH,
    int height = DEFAULT_WIN_HEIGHT,
    uint32_t window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
    uint32_t renderer_flags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
GPU_Target* get_renderer();
void set_renderer(GPU_Target*);
GPU_Target* get_target();
Uint32 get();

void render();
void clear();
}

#endif // DESIGNER_HPP
