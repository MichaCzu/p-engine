#include "pge/draw.hpp"
#include "pge/constants.hpp"
#include "pge/debug.hpp"
#include <SDL2/SDL.h>

#include <iostream>
#include <math.h>
#include <vector>

GPU_Target* cTarget;
GPU_Target* cCTarget;
Uint32 cWindow = 0;
SDL_Color cColor = { 255, 255, 255, 255 };
float cScale;
uint16_t cDetail;

namespace pge {
	namespace draw {

SDL_Color get_pixel(Sint16 x, Sint16 y)
{
    return GPU_GetPixel(cCTarget, x, y);
}
// color -  0xRRGGBBAA
void pixel(float _x, float _y, SDL_Color _color) { GPU_Pixel(cCTarget, _x, _y, _color); }
void rect(float _x, float _y, float _w, float _h, SDL_Color _color) { GPU_Rectangle(cCTarget, _x, _y, _x + _w, _y + _h, _color); }
void rectF(float _x, float _y, float _w, float _h, SDL_Color _color) { GPU_RectangleFilled(cCTarget, _x, _y, _x + _w, _y + _h, _color); }
void line(float _x, float _y, float _xx, float _yy, SDL_Color _color) { GPU_Line(cCTarget, _x, _y, _xx, _yy, _color); }
void rectR(float _x, float _y, float _w, float _h, float _r, SDL_Color _color) { GPU_RectangleRound(cCTarget, _x, _y, _x + _w, _y + _h, _r, _color); }
void rectRF(float _x, float _y, float _w, float _h, float _r, SDL_Color _color) { GPU_RectangleRoundFilled(cCTarget, _x, _y, _x + _w, _y + _h, _r, _color); }
void circle(float _x, float _y, float _r, SDL_Color _color) { GPU_Circle(cCTarget, _x, _y, _r, _color); }
void circleF(float _x, float _y, float _r, SDL_Color _color) { GPU_CircleFilled(cCTarget, _x, _y, _r, _color); }
void sector(float _x, float _y, float _ir, float _or, float _sa, float _ea, SDL_Color _color) { GPU_Sector(cCTarget, _x, _y, _ir, _or, _sa, _ea, _color); }
void sectorF(float _x, float _y, float _ir, float _or, float _sa, float _ea, SDL_Color _color) { GPU_SectorFilled(cCTarget, _x, _y, _ir, _or, _sa, _ea, _color); }

void pixel(float _x, float _y) { GPU_Pixel(cCTarget, _x, _y, cColor); }
void rect(float _x, float _y, float _w, float _h) { GPU_Rectangle(cCTarget, _x, _y, _x + _w, _y + _h, cColor); }
void rectF(float _x, float _y, float _w, float _h) { GPU_RectangleFilled(cCTarget, _x, _y, _x + _w, _y + _h, cColor); }
void line(float _x, float _y, float _xx, float _yy) { GPU_Line(cCTarget, _x, _y, _xx, _yy, cColor); }
void rectR(float _x, float _y, float _w, float _h, float _r) { GPU_RectangleRound(cCTarget, _x, _y, _x + _w, _y + _h, _r, cColor); }
void rectRF(float _x, float _y, float _w, float _h, float _r) { GPU_RectangleRoundFilled(cCTarget, _x, _y, _x + _w, _y + _h, _r, cColor); }
void circle(float _x, float _y, float _r) { GPU_Circle(cCTarget, _x, _y, _r, cColor); }
void circleF(float _x, float _y, float _r) { GPU_CircleFilled(cCTarget, _x, _y, _r, cColor); }
void sector(float _x, float _y, float _ir, float _or, float _sa, float _ea) { GPU_Sector(cCTarget, _x, _y, _ir, _or, _sa, _ea, cColor); }
void sectorF(float _x, float _y, float _ir, float _or, float _sa, float _ea) { GPU_SectorFilled(cCTarget, _x, _y, _ir, _or, _sa, _ea, cColor); }

void set_scale_factor(uint32_t _w, uint32_t _h, uint16_t _factor) // WIP
{
    float _wScale = _w / _factor;
    float _hScale = _h / _factor;

    if (_hScale >= _wScale)
        set_scale(_wScale);
    else
        set_scale(_hScale);
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

namespace viewport {
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

bool contains(float _x, float _y, View* _vp)
{
    if (_x < _vp->x && _x > _vp->x + _vp->w && _y < _vp->y && _y > _vp->x + _vp->h)
        return true;
    else
        return false;
}

bool contains(float _x, float _y, float _w, float _h, View* _vp)
{
    if (_x < _vp->x && _x + _w > _vp->x + _vp->w && _y < _vp->y && _y + _h > _vp->x + _vp->h)
        return true;
    else
        return false;
}
}

namespace window {
void create(int width, int height, uint32_t window_flags,
    uint32_t renderer_flags)
{
    SDL_Window* window;
	//GPU_SetPreInitFlags( GPU_INIT_USE_ROW_BY_ROW_TEXTURE_UPLOAD_FALLBACK );
    GPU_SetRequiredFeatures(GPU_FEATURE_ALL_BASE);
    pge::debug::log("flags set");
    
	cTarget = GPU_InitRenderer(GPU_RENDERER_OPENGL_2, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    pge::debug::log("window created");
    SDL_SetWindowMinimumSize(SDL_GetWindowFromID(cTarget->context->windowID), DEFAULT_WIN_MINWIDTH, DEFAULT_WIN_MINHEIGHT);
    if (DEFAULT_WIN_MAXWIDTH > 0 && DEFAULT_WIN_MAXHEIGHT > 0)
        SDL_SetWindowMaximumSize(SDL_GetWindowFromID(cTarget->context->windowID), DEFAULT_WIN_MAXWIDTH, DEFAULT_WIN_MAXHEIGHT);

    cCTarget = cTarget;
}

void rename(std::string name)
{
    SDL_SetWindowTitle(SDL_GetWindowFromID(cTarget->context->windowID), name.c_str());
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

int w()
{
    return cTarget->context->window_w;
}

int h()
{
    return cTarget->context->window_h;
}

void resize(int width, int height)
{
    //cTarget->context->window_w = width;
    //cTarget->context->window_h = height;
    //SDL_SetWindowSize(SDL_GetWindowFromID(cTarget->context->windowID), width, height);
    //set_resolution(width, height);
}

void set_resolution(int width, int height)
{
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
