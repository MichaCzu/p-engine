#include "pge/image.hpp"
#include "SDL2/SDL.h"
#include "pge/core.hpp"
#include "pge/crypt.hpp"
#include "pge/debug.hpp"
#include "pge/draw.hpp"
#include <SDL2/SDL_gpu.h>
#include <vector>

namespace pge::image {

struct StructTexture {
    GPU_Image* text;
    std::string path;
    int access;
    int16_t width;
    int16_t height;
    uint8_t alpha;
};

static std::vector<StructTexture*> vTexture;
static uint16_t find_first_free();

uint16_t load(std::string _path)
// keying - 0xRRGGBBAA
{
    SDL_Surface* loadSurface = IMG_Load(_path.c_str());
    uint16_t _id = (uint16_t)-1;
    debug::log("Loading " + _path);
	if (!loadSurface) {
        if (pge::is_running())
            debug::prompt("Missing file", "Couldn't load" + _path + " | reason: " + IMG_GetError(), 2);
        debug::log("Aborting...");

        return (uint16_t)-1;
    } else {
        _id = find_first_free();
        if (_id == (uint16_t)-1) {
            vTexture.push_back(new StructTexture);
            _id = vTexture.size() - 1;
        }
        vTexture[_id]->text = GPU_CopyImageFromSurface(loadSurface);
        GPU_SetImageFilter(vTexture[_id]->text, GPU_FILTER_NEAREST);
        GPU_SetSnapMode(vTexture[_id]->text, GPU_SNAP_NONE);
        //GPU_SetBlendMode(vTexture[_id]->text, GPU_BLEND_NORMAL);
        SDL_FreeSurface(loadSurface);

        if (vTexture[_id]->text == NULL) {
            return (uint16_t)-1;
        } else {
            vTexture[_id]->width = loadSurface->w;
            vTexture[_id]->height = loadSurface->h;
            vTexture[_id]->alpha = 255;
        }
    }

    return _id;
}

uint16_t load_ex(std::string _path, char* key)
{
    pge::crypt::file(_path);
    uint16_t flag = load(_path);
    pge::crypt::file(_path);
    return flag;
}

bool load_target(uint16_t text)
{
    GPU_LoadTarget(vTexture[text]->text);
    return true;
}

uint16_t create(uint16_t w, uint16_t h)
{
    uint16_t _id = find_first_free();
    if (_id == (uint16_t)-1) {
        vTexture.push_back(new StructTexture);
        _id = vTexture.size() - 1;
    }

    vTexture[_id]->text = GPU_CreateImage(w, h, GPU_FORMAT_RGBA);

    if (vTexture[_id]->text == NULL) {
        return (uint16_t)-1;
    } else {
        GPU_LoadTarget(vTexture[_id]->text);
        GPU_SetImageFilter(vTexture[_id]->text, GPU_FILTER_NEAREST);
        GPU_SetSnapMode(vTexture[_id]->text, GPU_SNAP_POSITION_AND_DIMENSIONS);
        vTexture[_id]->width = w;
        vTexture[_id]->height = h;
        vTexture[_id]->access = SDL_TEXTUREACCESS_TARGET;
        vTexture[_id]->alpha = 255;
        return _id;
    }
}

// TODO: freeing texture by _path
bool free(std::string _path)
{
    return false;
}

bool free(uint16_t _id)
{
    if (vTexture.size() >= _id || vTexture[_id]->text != NULL) {
        GPU_FreeImage(vTexture[_id]->text);
        vTexture[_id]->text = NULL;
        vTexture[_id]->width = 0;
        vTexture[_id]->height = 0;
        vTexture[_id]->alpha = 255;
        vTexture[_id]->path = "";
        return true;
    } else
        return false;
}

bool is_free(uint16_t _id)
{
    if (vTexture.size() > _id || vTexture[_id]->text == NULL)
        return true;
    else
        return false;
}

void purgeall()
{
    for (int i = vTexture.size() - 1; i >= 0; i--) {
        image::free((uint16_t)i);
        vTexture.pop_back();
    }
}

bool draw(uint16_t _id, int32_t _x, int32_t _y, const Rect* _clip,
    const Pixel* _center, float _angle, float _xscale, float _yscale,
    GPU_FlipEnum _flip)
{
    GPU_Rect* clip = NULL;
    GPU_Rect renderQuad;
    int px = 0, py = 0;

    if (_clip != NULL) {
        clip = new GPU_Rect;
        clip->x = _clip->x;
        clip->y = _clip->y;
        clip->w = _clip->w;
        clip->h = _clip->h;
        renderQuad = GPU_MakeRect(_x, _y, clip->w, clip->h);
    } else
        renderQuad = GPU_MakeRect(_x, _y, vTexture[_id]->width, vTexture[_id]->height);
    if (_center != NULL) {
        px = _center->x;
        py = _center->y;
    } else if (clip != NULL) {
        px = clip->w / 2;
        py = clip->h / 2;
    }
    if (_xscale != 1 || _yscale != 1) {
        renderQuad.w = renderQuad.w * _xscale;
        renderQuad.h = renderQuad.h * _yscale;
    }

    GPU_BlitRectX(vTexture[_id]->text, clip, window::get_renderer(), &renderQuad, _angle, px, py, _flip);
    if (clip != NULL) {
        delete clip;
    }

    return true;
}

void set_colormod(uint16_t _id, SDL_Color _color)
{
    GPU_SetColor(vTexture[_id]->text, _color);
}

void set_blendeq(uint16_t _id, GPU_BlendEqEnum color, GPU_BlendEqEnum alpha)
{
    //GPU_SetBlendEquation(vTexture[_id]->text, color, alpha);
}

void set_blendfunc(uint16_t _id, GPU_BlendFuncEnum source_color, GPU_BlendFuncEnum dest_color, GPU_BlendFuncEnum source_alpha, GPU_BlendFuncEnum dest_alpha)
{
    //GPU_SetBlendFunction(vTexture[_id]->text, source_color, dest_color, source_alpha, dest_alpha);
}

void set_blendmode(uint16_t _id, GPU_BlendPresetEnum blending)
{
    //GPU_SetBlendMode(vTexture[_id]->text, blending);
}

void set_filter(uint16_t _id, GPU_FilterEnum filter)
{
    GPU_SetImageFilter(vTexture[_id]->text, filter);
}

bool query(uint16_t _id, int w, int h)
{
    if (_id < vTexture.size()) {
        GPU_Image* texture = GPU_CreateImage(w, h, GPU_FORMAT_RGBA);

        if (texture == NULL) {
            return false;
        } else {
            GPU_FreeImage(vTexture[_id]->text);
            vTexture[_id]->text = texture;
            GPU_LoadTarget(vTexture[_id]->text);
            GPU_GenerateMipmaps(vTexture[_id]->text);
            vTexture[_id]->width = w;
            vTexture[_id]->height = h;
            vTexture[_id]->alpha = 255;
            return true;
        }
    }
    return false;
}

bool set_target(uint16_t target)
{
    if (target < vTexture.size()) {
        pge::window::set_renderer(vTexture[target]->text->target);
        return true;
    } else
        return false;
}

void reset_target()
{
    pge::window::set_renderer(pge::window::get_target());
}

std::string get_path(uint16_t _id)
{
    return vTexture[_id]->path;
}
uint16_t get_width(uint16_t _id)
{
    return vTexture[_id]->width;
}
uint16_t get_height(uint16_t _id)
{
    return vTexture[_id]->height;
}

uint8_t get_alpha(uint16_t _id)
{
    uint8_t flag;
    //SDL_GetTextureAlphaMod(vTexture[_id]->text, &flag);
    return flag;
}

//////Private////

static uint16_t find_first_free()
{
    for (int i = 0; i < vTexture.size(); i++) {
        if (vTexture[i]->text == NULL) {
            return i;
        }
    }
    return (uint16_t)-1;
}
/////////////////
}
