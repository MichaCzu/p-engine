#ifndef ECONTROLLER_HPP
#define ECONTROLLER_HPP

#include "block.hpp"
#include "pge/pge.hpp"
#include "tools/worldgen.hpp"
#include <iostream>
#include <vector>
//
struct sctBlock {
    Block* type;
    Block* back = nullptr;
    pge::Rect* clip = nullptr;
    pge::Rect* side_u = nullptr;
    pge::Rect* side_d = nullptr;
    pge::Rect* side_l = nullptr;
    pge::Rect* side_r = nullptr;
    bool corner = true;
    bool corner_ul = false;
    bool corner_ur = false;
    bool corner_bl = false;
    bool corner_br = false;
    bool is0 = false;

    pge::Rect* bclip = nullptr;
    pge::Rect* bside_u = nullptr;
    pge::Rect* bside_d = nullptr;
    pge::Rect* bside_l = nullptr;
    pge::Rect* bside_r = nullptr;
    bool bcorner = true;
    bool bcorner_ul = false;
    bool bcorner_ur = false;
    bool bcorner_bl = false;
    bool bcorner_br = false;
    bool bis0 = false;
    int8_t lightlevel = 0;

    sctBlock(Block* _type, Block* _bg = nullptr)
    {
        type = _type;
        back = _bg;
    }
    void tile(uint8_t tile);
    void tilebg(uint8_t tile, uint8_t empty);
};

class LController;

class EController {
    std::vector<sctBlock*> aLevel;
    uint16_t aLevelWidth = 1000;
    uint16_t aLevelHeight = 10000;
    LController* lcontrol;

    int bg[5], bgsky, nightsky;
    void bg_load(std::string type);
    void bg_loadsky(std::string type);
    void bg_loadskynight(std::string type);

    bool bkchk(uint32_t id);
    bool bkchk_m(uint32_t id, uint32_t id2);
    uint8_t bkchk_t(uint32_t id);
    bool bgchk(uint32_t id);
    bool bgchk_m(uint32_t id, uint32_t id2);
    uint8_t bgchk_t(uint32_t id);
    bool bgchk_f(uint32_t id, uint32_t id2);

protected:
    friend class Controller;
    void bc_update(); //slow
    EController(LController* control, std::string bgtype, std::string skytype)
    {
        lcontrol = control;
        bg_load(bgtype);
        bg_loadsky(skytype);
        bg_loadskynight("sky1");
        WorldGen newworldgen(0, aLevelWidth, aLevelHeight);
        std::vector<Block*>* newmap = newworldgen.generate();
        load_map(newmap, aLevelWidth, aLevelHeight);

        delete newmap;
    }

    ~EController()
    {
        for (int i = 0; i < aLevel.size(); i++) {
            if (aLevel[i] != nullptr)
                delete aLevel[i];
        }
    }

    void load_map(std::vector<Block*>* map, uint16_t aLevelWidth, uint16_t aLevelHeight);

    void bg_render(int ypos, pge::viewport::View& vp, int time);
    void bc_render(pge::viewport::View& vp, std::vector<int>* bgfiles);
    void bc_renderbg(pge::viewport::View& vp, std::vector<int>* bgfiles);

public:
    static void env_draw_background(int bg, float parallax, int ypos, pge::viewport::View vp, uint32_t c1 = 0x00000000);
    static void env_draw_background_sky(int bg, uint32_t c1, uint32_t c2, int time);

    void draw_block(sctBlock* block, int xx, int yy);
    void draw_blockbg(sctBlock* block, int xx, int yy);

    void bc_place_block(uint8_t _ID, uint32_t x, uint32_t y, bool bg = 0);
    void bc_remove_block(uint32_t x, uint32_t y);

    void bc_update(int32_t x, int32_t y, uint16_t w, uint16_t h);

    void bc_update(uint32_t x, uint32_t y);
    uint16_t bc_check(uint32_t x, uint32_t y);
    uint16_t bg_check(uint32_t x, uint32_t y);
    int8_t light_check(uint32_t x, uint32_t y);
    Block* bc_type(uint32_t x, uint32_t y, bool bg = false);

    uint16_t get_mapWidth() { return aLevelWidth; }
    uint16_t get_mapHeight() { return aLevelHeight; }
};

#endif
