#include "pge/draw.hpp"
#include "pge/constants.hpp"
#include <SDL2/SDL.h>

#include <iostream>
#include <math.h>
#include <vector>

static GPU_Target* cTarget;
static GPU_Target* cCTarget;
static Uint32 cWindow = 0;
static SDL_Color cColor = { 255, 255, 255, 255 };
static float cScale;
static uint16_t cDetail;

namespace pge::draw {

SDL_Color get_pixel(Sint16 x, Sint16 y)
{
    return GPU_GetPixel(cCTarget, x, y);
}
// color -  0xRRGGBBAA
void pixel(int32_t _x, int32_t _y, SDL_Color _color)
{
    GPU_Pixel(cCTarget, _x, _y, _color);
}

void rect(int32_t _x, int32_t _y, int32_t _w, int32_t _h,
    SDL_Color _color)
{
    GPU_Rectangle(cCTarget, _x, _y, _x + _w, _y + _h, _color);
}

void rectFill(int32_t _x, int32_t _y, int32_t _w, int32_t _h,
    SDL_Color _color)
{
    GPU_RectangleFilled(cCTarget, _x, _y, _x + _w, _y + _h, _color);
}

void line(int32_t _x, int32_t _y, int32_t _xx, int32_t _yy,
    SDL_Color _color)
{
    GPU_Line(cCTarget, _x, _y, _xx, _yy, _color);
}

void set_scale_factor(uint32_t _w, uint32_t _h, uint16_t _factor) // WIP
{
    float _wScale = _w / _factor;
    float _hScale = _h / _factor;

    if (_hScale >= _wScale)
        draw::set_scale(_wScale);
    else
        draw::set_scale(_hScale);
}

void set_scale(float _scale)
{
    cScale = round(_scale * cDetail) / cDetail;
}

void set_scale_detail(uint8_t _detail)
{
    switch (_detail) {
    case 0:
        cDetail = 1;
        break;
    case 1:
        cDetail = 10;
        break;
    case 2:
        cDetail = 100;
        break;
    default:
        cDetail = 10000;
        break;
    }
}

float get_scale()
{
    return cScale;
}
}

namespace pge::viewport {
float get_xmoved(float _x, View* _vp)
{
    return (_x - _vp->x);
}

float get_xmoved_parallax(float _x, float _factor, float _sx, View* _vp)
{
    return (_sx - _vp->x) + (_x + (_vp->x + _vp->w / 2) - _sx) * _factor;
}

float get_ymoved(float _y, View* _vp)
{
    return (_y - _vp->y);
}

float get_ymoved_parallax(float _y, float _factor, float _sy, View* _vp)
{
    return (_sy - _vp->y) + (_y + (_vp->y + _vp->h / 3 * 2) - _sy) * _factor;
}

bool contains_point(float _x, float _y, View* _vp)
{
    if (_x < _vp->x && _x > _vp->x + _vp->w && _y < _vp->y && _y > _vp->x + _vp->h)
        return true;
    else
        return false;
}

bool contains_rect(float _x, float _y, float _w, float _h, View* _vp)
{
    if (_x < _vp->x && _x + _w > _vp->x + _vp->w && _y < _vp->y && _y + _h > _vp->x + _vp->h)
        return true;
    else
        return false;
}
}

namespace pge::window {
void create(int width, int height, uint32_t window_flags,
    uint32_t renderer_flags)
{
    SDL_Window* window;
    GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);
    GPU_SetRequiredFeatures(GPU_FEATURE_ALL_BASE);
    //window = SDL_CreateWindow("GameTest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    //GPU_SetInitWindow(SDL_GetWindowID(window));
    cTarget = GPU_InitRenderer(GPU_RENDERER_OPENGL_3, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    cCTarget = cTarget;
}

void render()
{
    GPU_Flip(cTarget);
}

void clear()
{
    GPU_Clear(cCTarget);
}

GPU_Target* get_renderer()
{
    return cCTarget;
}

void set_renderer(GPU_Target* target)
{
    cCTarget = target;
}

GPU_Target* get_target()
{
    return cTarget;
}

Uint32 get()
{
    return cWindow;
}

void get_size(int* width, int* height)
{
    *width = cTarget->context->window_w;
    *height = cTarget->context->window_h;
}

void set_size(int width, int height)
{
    cTarget->context->window_w = width;
    cTarget->context->window_h = height;
    GPU_SetWindowResolution(width, height);
}

bool is_fullscreen()
{
    if (GPU_GetFullscreen())
        return true;
    else
        return false;
}

void set_fullscreen(bool var)
{
    if (var) {
        GPU_SetFullscreen(true, true);
    } else
        GPU_SetFullscreen(false, false);
}
}
////////////Private////////////////////////////////////////////////////////////////////////////////////////////////

//static void
//sort_viewports()
//{
//for (int i = 1; i < vViewports.size(); i++) {
//for (int j = 0; j < vViewports.size(); j++) {
//if (vViewports[j].id > vViewports[j + 1].id) {
//auto buffer = vViewports[j].id;
//vViewports[j].id = vViewports[j + 1].id;
//vViewports[j + 1].id = buffer;
//}
//}
//}
//}
