// Loading, managing and drawing sprites on screen.
#ifndef PGE_IMAGE_HPP
#define PGE_IMAGE_HPP

#include "types.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <string>

namespace pge::image {

uint16_t load(std::string _path);
uint16_t create(uint16_t w, uint16_t h);
bool free(std::string _path);
bool free(uint16_t _id);
bool is_free(uint16_t _id);
void purgeall();

bool draw(uint16_t _id, int32_t _x, int32_t _y,
    const Rect* _clip = NULL, const Pixel* _center = NULL,
    float _angle = 0, float _xscale = 1, float _yscale = 1,
    GPU_FlipEnum _flip = 0);
void set_blendmode(uint16_t _id, GPU_BlendPresetEnum blending);
void set_filter(uint16_t _id, GPU_FilterEnum filter);
void set_blendeq(uint16_t _id, GPU_BlendEqEnum color, GPU_BlendEqEnum alpha);
void set_blendfunc(uint16_t _id, GPU_BlendFuncEnum source_color, GPU_BlendFuncEnum dest_color, GPU_BlendFuncEnum source_alpha, GPU_BlendFuncEnum dest_alpha);
void set_colormod(uint16_t _id, SDL_Color color);
bool query(uint16_t _id, int w, int h);

bool set_target(uint16_t target);
bool load_target(uint16_t text);
void reset_target();

std::string get_path(uint16_t _id);
uint16_t get_width(uint16_t _id);
uint16_t get_height(uint16_t _id);
uint8_t get_alpha(uint16_t _id);
}

#endif // RESOURCER_HPP
