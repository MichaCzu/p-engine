#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include "pge/types.hpp"
#include <stdint.h>
#include <string>

bool aBlocks_Init();
pge::Rect* aBlock_getclip(uint8_t type, uint8_t variation);

enum BlockID {
    b_air = 0,
    b_grass = 1,
    b_dirt,
    b_rock,
    b_coal,
    b_iron,
    b_hardrock,
    b_hardrockgrass,
    b_plank,
    b_log,
    b_glass,
    b_ladder,
    b_platform,
    b_pplatform,
    b_water,
};

enum BlockType {
    bt_normal = 0,
    bt_transp,
    bt_stairs,
    bt_stair1,
    bt_stair2,
    bt_platfm,
    bt_water,
    bt_ladder,
    bt_rail,
    bt_rail_b,
    bt_empty = 255,
};

struct Block {
    BlockType type;
    bool draw_bg;
    bool draw_front;
    BlockID ID;
    uint16_t match;
    std::string image_path;
    std::string imagebg_path = "";

    //Modificators
    float speed_mod = 1;
    float spdup_mod = 1;
    float spdslow_mod = 1;
    float jump_mod = 1;
    float jumpspd_mod = 1;
    float bouncy_mod = 0;
    float falldmg_mod = 1;
    bool damage = 100;
    //values
    bool direction = 0;
    uint16_t image = 0;
    uint16_t imagebg = 0;
};

extern Block aBlocks[];

#endif

//blocks
//
//categories:
//1 - normal
//2 - stairs1 2
//3 - stairs2a 4001-5000
//4 - stairs2b
//5 - platform 501-2000
//6 - ladders
//7 - rail
//8 - rail_bumper
//last - empty
