#ifndef WORLDGEN_HPP
#define WORLDGEN_HPP

#include "app/game/block.hpp"
#include "pge/pge.hpp"
#include <SDL2/SDL.h>
#include <vector>

class WorldGen {
    uint32_t seed;

    uint16_t type;
    uint16_t aLevelWidth;
    uint16_t aLevelHeight;

    //world generation settings
    //goes here

public:
    WorldGen(uint16_t _type, uint16_t width, uint16_t height)
    {
        type = _type;
        aLevelWidth = width;
        aLevelHeight = height;
    }
    std::vector<Block*>* generate();

    //worldgen settings setters
    //goes here
};

#endif //WORLDGEN
