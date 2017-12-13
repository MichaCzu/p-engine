#include "app/game/block.hpp"
#include "pge/crypt.hpp"
#include "pge/image.hpp"
#include "pge/types.hpp"
#include <stdint.h>

//speed, spdup, spdslow, jump, jumpspd, bouncy, falldmg
Block aBlocks[] = {
    { bt_empty, 1, 0, b_air, 0, "", "" },
    { bt_normal, 0, 1, b_grass, 1, "res/spr/env/grass.png", "res/spr/env/grassbg.png" },
    { bt_normal, 0, 1, b_dirt, 1, "res/spr/env/dirt.png", "res/spr/env/dirtbg.png" },
    { bt_normal, 0, 1, b_rock, 2, "res/spr/env/rock.png", "res/spr/env/rockbg.png" },
    { bt_normal, 0, 1, b_coal, 2, "res/spr/env/rock_coal.png", "res/spr/env/rock_coalbg.png" },
    { bt_normal, 0, 1, b_iron, 2, "res/spr/env/rock_iron.png", "res/spr/env/rock_ironbg.png" },
    { bt_normal, 0, 1, b_hardrock, 3, "res/spr/env/hardrock.png", "res/spr/env/hardrockbg.png" },
    { bt_normal, 0, 1, b_hardrockgrass, 3, "res/spr/env/hardrockgrass.png", "res/spr/env/hardrockbg.png" },
    { bt_normal, 0, 1, b_plank, 4, "res/spr/env/plank.png", "res/spr/env/plankbg.png" },
    { bt_normal, 1, 0, b_log, 5, "res/spr/env/log.png", "res/spr/env/logbg.png" },
    { bt_transp, 1, 1, b_glass, 110, "res/spr/env/glass.png", "res/spr/env/glassbg.png" },
    { bt_ladder, 1, 0, b_ladder, 101, "res/spr/env/ladder.png", "res/spr/env/ladder.png" },
    { bt_platfm, 1, 0, b_platform, 1, "res/spr/env/grassplatform.png", "res/spr/env/grassplatform.png" },
    { bt_platfm, 1, 0, b_pplatform, 102, "res/spr/env/pplatfm.png", "res/spr/env/pplatfm.png" },
    { bt_water, 1, 1, b_water, 111, "res/spr/env/water.png", "res/spr/env/water.png", 1, 1, 0.1 },
    //{ bt_normal, 1, 1, b_grass, 4, "res/spr/env/sloped.png", "res/spr/env/grassbg.png" },
};

bool aBlocks_Init()
{
    for (int i = 0; i < (sizeof(aBlocks) / sizeof(*aBlocks)); i++) {
        if (aBlocks[i].image_path != "") {
            uint16_t flag = pge::image::load(aBlocks[i].image_path);
            if (flag < (uint16_t)(-1))
                aBlocks[i].image = flag;
            else
                return 0;
        }

        if (aBlocks[i].imagebg_path == "")
            continue;

        uint16_t flag2 = pge::image::load(aBlocks[i].imagebg_path);
        if (flag2 < (uint16_t)(-1)) {
            aBlocks[i].imagebg = flag2;
        } else
            return 0;
    }
    return 1;
}

pge::Rect blockClip_0[8] = {
    { 0, 48, 12, 12 }, { 12, 48, 12, 12 }, { 24, 48, 12, 12 }, { 36, 48, 12, 12 }, { 0, 60, 12, 12 }, { 12, 60, 12, 12 }, { 24, 60, 12, 12 }, { 36, 60, 12, 12 },
},
          blockClip_1_7[7][4] = {
              {
                  { 0, 0, 12, 12 }, { 12, 0, 12, 12 }, { 24, 0, 12, 12 }, { 36, 0, 12, 12 },
              },
              {
                  { 0, 12, 12, 12 }, { 12, 12, 12, 12 }, { 24, 12, 12, 12 }, { 36, 12, 12, 12 },
              },
              {
                  { 0, 24, 12, 12 }, { 12, 24, 12, 12 }, { 24, 24, 12, 12 }, { 36, 24, 12, 12 },
              },
              {
                  { 0, 36, 12, 12 }, { 12, 36, 12, 12 }, { 24, 36, 12, 12 }, { 36, 36, 12, 12 },
              },
              {
                  { 0, 72, 12, 12 }, { 12, 72, 12, 12 }, { 24, 72, 12, 12 }, { 36, 72, 12, 12 },
              },
              {
                  { 0, 84, 12, 12 }, { 12, 84, 12, 12 }, { 24, 84, 12, 12 }, { 36, 84, 12, 12 },
              },
              {
                  { 0, 96, 12, 12 }, { 12, 96, 12, 12 }, { 24, 96, 12, 12 }, { 36, 96, 12, 12 },
              },
          },
          blockClip_8_15[8][2] = {
              {
                  { 48, 12, 12, 12 }, { 60, 12, 12, 12 },
              },
              {
                  { 72, 12, 12, 12 }, { 84, 12, 12, 12 },
              },
              {
                  { 48, 24, 12, 12 }, { 60, 24, 12, 12 },
              },
              {
                  { 72, 24, 12, 12 }, { 84, 24, 12, 12 },
              },
              {
                  { 48, 36, 12, 12 }, { 60, 36, 12, 12 },
              },
              {
                  { 72, 36, 12, 12 }, { 84, 36, 12, 12 },
              },
              {
                  { 48, 48, 12, 12 }, { 60, 48, 12, 12 },
              },
              {
                  { 72, 48, 12, 12 }, { 84, 48, 12, 12 },
              },
          },
          blockClip_16_20[5][4] = {
              {
                  { 48, 0, 12, 12 }, { 60, 0, 12, 12 }, { 72, 0, 12, 12 }, { 84, 0, 12, 12 },
              },
              {
                  { 48, 60, 12, 12 }, { 60, 60, 12, 12 }, { 72, 60, 12, 12 }, { 84, 60, 12, 12 },
              },
              {
                  { 48, 72, 12, 12 }, { 60, 72, 12, 12 }, { 72, 72, 12, 12 }, { 84, 72, 12, 12 },
              },
              {
                  { 48, 84, 12, 12 }, { 60, 84, 12, 12 }, { 72, 84, 12, 12 }, { 84, 84, 12, 12 },
              },
              {
                  { 48, 96, 12, 12 }, { 60, 96, 12, 12 }, { 72, 96, 12, 12 }, { 84, 96, 12, 12 },
              },
          };

pge::Rect* aBlock_getclip(uint8_t type, uint8_t variation)
{
    if (type == 0 && pge::random::get(0, 100) < 40)
        variation = 0;

    switch (type) {
    case 0:
        return &blockClip_0[variation % 8];
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        return &blockClip_1_7[(type - 1) % 7][variation % 4];
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        return &blockClip_8_15[(type - 8) % 8][variation % 2];
        break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        return &blockClip_16_20[(type - 16) % 5][variation % 4];
        break;
    default:
        return nullptr;
    }
}
