#include "app/game/enviroment_controller.hpp"
#include "app/game/tools/light_controller.hpp"
#include <SDL2/SDL.h>
#include <vector>
//
void EController::env_draw_background(int bg, float parallax, int ypos, pge::viewport::View vp, uint32_t c)
{
    int w, h;
    pge::window::get_size(&w, &h);
    float scale = pge::draw::get_scale();

    int iw = pge::image::get_width(bg);
    int ih = pge::image::get_height(bg);
    float _x = pge::viewport::get_xmoved_parallax(0, parallax, 0, &vp);
    float _y = pge::viewport::get_ymoved_parallax(0, parallax * 0.8, ypos, &vp);

    for (int i = -1; i <= w / iw * scale + 1; i++) {
        pge::image::draw(bg,
            fmod(_x, iw) * scale + iw * scale * i,
            (_y - ih + 120) * scale,
            NULL, NULL, 0, scale, scale);
    }
    if (c > 0)
        pge::draw::rectFill(0, (_y + 120) * scale, w, h - (_y - ih) * scale, SDL_Color{ 0x44, 0x22, 0xAA, 0xFF });
}

void EController::env_draw_background_sky(int bg, uint32_t c1, uint32_t c2, int _time)
{
    static int rest = 0;
    static int move = 0;
    move += (_time + rest) / 200;
    rest = (rest + _time) % 200;

    int scale = pge::draw::get_scale();
    int w, h;
    pge::window::get_size(&w, &h);

    pge::draw::rectFill(0, 0, w, h / 3 - 180 * scale, SDL_Color{ 0x44, 0x22, 0xAA, 0xFF });
    pge::draw::rectFill(0, h / 3 + 180 * scale, w, h / 3 * 2 - 180 * scale, SDL_Color{ 0x44, 0x22, 0xAA, 0xFF });

    for (int i = -1; i <= w / 640 * scale; i++) {
        pge::image::draw(bg,
            (move % (640 * scale)) + 640 * scale * i,
            h / 3 - 180 * scale,
            NULL, NULL, 0, scale, scale);
    }
}

void EController::bg_load(std::string type)
{
    for (int i = 0; i < 5; i++) {
        bg[i] = pge::image::load("res/spr/bg/" + type + "/" + std::to_string(i) + ".png");
    }
}

void EController::bg_loadsky(std::string type)
{
    bgsky = pge::image::load("res/spr/bg/skies/" + type + ".png");
}

void EController::bg_loadskynight(std::string type)
{
    nightsky = pge::image::load("res/spr/bg/skies/" + type + ".png");
}

void EController::draw_block(sctBlock* block, int xx, int yy)
{
    uint16_t image = block->type->image;
    pge::image::draw(image, xx, yy, block->clip, nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());

    if (block->corner) {
        if (block->corner_ul)
            pge::image::draw(image, xx, yy, aBlock_getclip(16, 3), nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
        if (block->corner_ur)
            pge::image::draw(image, xx, yy, aBlock_getclip(16, 0), nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
        if (block->corner_bl)
            pge::image::draw(image, xx, yy, aBlock_getclip(16, 2), nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
        if (block->corner_br)
            pge::image::draw(image, xx, yy, aBlock_getclip(16, 1), nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
    }

    if (block->is0)
        return;

    uint16_t gap = 12 * pge::draw::get_scale();

    if (block->side_u != nullptr)
        pge::image::draw(image, xx, yy - gap, block->side_u, nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
    if (block->side_d != nullptr)
        pge::image::draw(image, xx, yy + gap, block->side_d, nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
    if (block->side_l != nullptr)
        pge::image::draw(image, xx - gap, yy, block->side_l, nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
    if (block->side_r != nullptr)
        pge::image::draw(image, xx + gap, yy, block->side_r, nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
}

void EController::draw_blockbg(sctBlock* block, int xx, int yy)
{
    uint16_t image = block->back->imagebg;
    pge::image::draw(image, xx, yy, block->bclip, nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());

    if (block->bcorner) {
        if (block->bcorner_ul)
            pge::image::draw(image, xx, yy, aBlock_getclip(16, 3), nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
        if (block->bcorner_ur)
            pge::image::draw(image, xx, yy, aBlock_getclip(16, 0), nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
        if (block->bcorner_bl)
            pge::image::draw(image, xx, yy, aBlock_getclip(16, 2), nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
        if (block->bcorner_br)
            pge::image::draw(image, xx, yy, aBlock_getclip(16, 1), nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
    }

    if (block->bis0)
        return;

    uint16_t gap = 12 * pge::draw::get_scale();

    if (block->bside_u != nullptr)
        pge::image::draw(image, xx, yy - gap, block->bside_u, nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
    if (block->bside_d != nullptr)
        pge::image::draw(image, xx, yy + gap, block->bside_d, nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
    if (block->bside_l != nullptr)
        pge::image::draw(image, xx - gap, yy, block->bside_l, nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
    if (block->bside_r != nullptr)
        pge::image::draw(image, xx + gap, yy, block->bside_r, nullptr, 0, pge::draw::get_scale(), pge::draw::get_scale());
}

void EController::load_map(std::vector<Block*>* map, uint16_t _aLevelWidth, uint16_t _aLevelHeight)
{
    if (map->size() != _aLevelWidth * _aLevelHeight)
        pge::debug::log("couldn't load map, invalid map size");

    aLevelHeight = _aLevelHeight;
    aLevelWidth = _aLevelWidth;

    aLevel.resize(aLevelWidth * aLevelHeight);
    for (int i = 0; i < aLevel.size(); i++) {
        if ((*map)[i] != nullptr) {
            aLevel[i] = new sctBlock((*map)[i]);
        } else {
            aLevel[i] = nullptr;
        }
    }

    bc_update();
}

void EController::bc_place_block(uint8_t _ID, uint32_t x, uint32_t y, bool bg)
{
    if (x + y * aLevelWidth > aLevel.size())
        return;

    if (aLevel[y * aLevelWidth + x] == nullptr) {
        if (bg)
            aLevel[y * aLevelWidth + x] = new sctBlock(nullptr, &aBlocks[_ID]);
        else
            aLevel[y * aLevelWidth + x] = new sctBlock(&aBlocks[_ID]);
    } else {
        if (bg)
            aLevel[y * aLevelWidth + x]->back = &aBlocks[_ID];
        else
            aLevel[y * aLevelWidth + x]->type = &aBlocks[_ID];
    }
    bc_update(x - 1, y - 1, 3, 3);
}

void EController::bc_remove_block(uint32_t x, uint32_t y)
{
    if (x + y * aLevelWidth > aLevel.size())
        return;

    if (aLevel[y * aLevelWidth + x] != nullptr) {
        delete aLevel[y * aLevelWidth + x];
        aLevel[y * aLevelWidth + x] = nullptr;
        bc_update(x - 1, y - 1, 3, 3);
    }
}

void EController::bc_update(uint32_t x, uint32_t y)
{
    if (y > aLevelHeight)
        return;

    int32_t i = y * aLevelWidth + x;
    int32_t ii = 0;
    bool desflag = true;
    if (aLevel[i] == nullptr)
        return;

    if (aLevel[i]->type != nullptr) {
        desflag = false;
        bool a = 1, b = 1, c = 1, d = 1, e = 1, f = 1, g = 1, h = 1;

        ii = i + aLevelWidth + 1;
        if (ii < aLevel.size()) {
            if (!bkchk(ii))
                a = 0;
            else if (!bkchk_m(ii, i))
                a = 0;
        }

        ii = i + aLevelWidth;
        if (ii < aLevel.size()) {
            if (!bkchk(ii))
                b = 0;
            else if (!bkchk_m(ii, i))
                b = 0;
        }

        ii = i + aLevelWidth - 1;
        if (ii < aLevel.size()) {
            if (!bkchk(ii))
                c = 0;
            else if (!bkchk_m(ii, i))
                c = 0;
        }

        ii = i - 1;
        if (i - 1 >= 0) {
            if (!bkchk(ii))
                d = 0;
            else if (!bkchk_m(ii, i))
                d = 0;
        }

        ii = i - aLevelWidth - 1;
        if (ii >= 0) {
            if (!bkchk(ii))
                e = 0;
            else if (!bkchk_m(ii, i))
                e = 0;
        }

        ii = i - aLevelWidth;
        if (ii >= 0) {
            if (!bkchk(ii))
                f = 0;
            else if (!bkchk_m(ii, i))
                f = 0;
        }

        ii = i - aLevelWidth + 1;
        if (ii >= 0) {
            if (!bkchk(ii))
                g = 0;
            else if (!bkchk_m(ii, i))
                g = 0;
        }

        ii = i + 1;
        if (ii < aLevel.size()) {
            if (!bkchk(ii))
                h = 0;
            else if (!bkchk_m(ii, i))
                h = 0;
        }

        uint8_t tile = a + b * 0b10 + c * 0b100 + d * 0b1000 + e * 0b10000 + f * 0b100000 + g * 0b1000000 + h * 0b10000000;
        aLevel[i]->tile(tile);
    }

    if (aLevel[i]->back != nullptr) {
        desflag = false;
        bool a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;
        bool bb = 0, bd = 0, bf = 0, bh = 0;

        ii = i + aLevelWidth + 1;
        if (ii < aLevel.size()) {
            if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && !bgchk(ii))
                a = 1;
            else if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && bgchk(ii))
                a = 1;
        }

        ii = i + aLevelWidth;
        if (ii < aLevel.size()) {
            if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && !bgchk(ii)) {
                bb = 1;
                b = 1;
            } else if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && bgchk(ii)) {
                b = 1;
            }
        }

        ii = i + aLevelWidth - 1;
        if (ii < aLevel.size()) {
            if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && !bgchk(ii)) {
                c = 1;
            } else if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && bgchk(ii))
                c = 1;
        }

        ii = i - 1;
        if (ii >= 0) {
            if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && !bgchk(ii)) {
                bd = 1;
                d = 1;
            } else if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && bgchk(ii)) {
                if (!bgchk_m(ii, i))
                    bd = 1;
                d = 1;
            }
        }

        ii = i - aLevelWidth - 1;
        if (ii >= 0) {
            if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && !bgchk(ii)) {
                e = 1;
            } else if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && bgchk(ii))
                e = 1;
        }

        ii = i - aLevelWidth;
        if (ii >= 0) {
            if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && !bgchk(ii)) {
                bf = 1;
                f = 1;
            } else if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && bgchk(ii)) {
                if (!bgchk_m(ii, i))
                    bf = 1;
                f = 1;
            }
        }

        ii = i - aLevelWidth + 1;
        if (ii >= 0) {
            if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && !bgchk(ii))
                g = 1;
            else if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && bgchk(ii))
                g = 1;
        }

        ii = i + 1;
        if (ii < aLevel.size()) {
            if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && !bgchk(ii)) {
                bh = 1;
                h = 1;
            } else if ((!bkchk(ii) || bkchk_t(ii) != bt_normal) && bgchk(ii)) {
                h = 1;
            }
        }

        if (desflag)
            bc_remove_block(x, y);

        uint8_t tile = a + b * 0b10 + c * 0b100 + d * 0b1000 + e * 0b10000 + f * 0b100000 + g * 0b1000000 + h * 0b10000000;
        uint8_t empty = bb * 0b1 + bd * 0b10 + bf * 0b100 + bh * 0b1000;
        aLevel[i]->tilebg(tile, empty);
    }

    //aLevel[i]->tile(0b10001111);
}

void EController::bc_update(int32_t x, int32_t y, uint16_t w, uint16_t h)
{
    while (x <= 0)
        x++;
    while (y <= 0)
        y++;

    for (int i = y; i < y + h; i++)
        for (int j = x; j < x + w; j++)
            bc_update(j, i);
}
void EController::bc_update()
{
    for (int i = 0; i < aLevelHeight; i++)
        for (int j = 0; j < aLevelWidth; j++)
            bc_update(j, i);
}

uint16_t EController::bc_check(uint32_t x, uint32_t y)
{
    if (x + y * aLevelWidth < aLevel.size()) {
        if (aLevel[x + y * aLevelWidth] != nullptr && aLevel[x + y * aLevelWidth]->type != nullptr) {
            return aLevel[x + y * aLevelWidth]->type->type;
        }
    }
    return bt_empty;
}

uint16_t EController::bg_check(uint32_t x, uint32_t y)
{
    if (x + y * aLevelWidth < aLevel.size()) {
        if (aLevel[x + y * aLevelWidth] != nullptr && aLevel[x + y * aLevelWidth]->back != nullptr) {
            return aLevel[x + y * aLevelWidth]->back->type;
        }
    }
    return bt_empty;
}

Block* EController::bc_type(uint32_t x, uint32_t y, bool bg)
{
    if (x + y * aLevelWidth < aLevel.size()) {
        if (aLevel[x + y * aLevelWidth] != nullptr && aLevel[x + y * aLevelWidth]->type != nullptr) {
            return aLevel[x + y * aLevelWidth]->type;
        }
    }
    return &aBlocks[0];
}

int8_t EController::light_check(uint32_t x, uint32_t y)
{
    if (x + y * aLevelWidth < aLevel.size()) {
        if (aLevel[x + y * aLevelWidth] != nullptr) {
            return aLevel[x + y * aLevelWidth]->lightlevel;
        }
    }
    return 127;
}

void EController::bc_renderbg(pge::viewport::View& vp, std::vector<int>* bgfiles)
{
    std::vector<int> files;
    for (int i = vp.y / 12; i < (vp.y + vp.h) / 12; i++)
        for (int j = vp.x / 12; j < (vp.x + vp.w) / 12; j++)
            if (aLevel[i * aLevelWidth + j] != nullptr) {
                if (aLevel[i * aLevelWidth + j]->type != nullptr && aLevel[i * aLevelWidth + j]->type->draw_bg) {
                    if (aLevel[i * aLevelWidth + j]->back != nullptr)
                        draw_blockbg(aLevel[i * aLevelWidth + j], (j * 12 - vp.x) * pge::draw::get_scale(), (i * 12 - vp.y) * pge::draw::get_scale());
                    if (!aLevel[i * aLevelWidth + j]->type->draw_front)
                        files.push_back(i * aLevelWidth + j);
                    else
                        bgfiles->push_back(i * aLevelWidth + j);
                } else if (aLevel[i * aLevelWidth + j]->type == nullptr && aLevel[i * aLevelWidth + j]->back != nullptr)
                    draw_blockbg(aLevel[i * aLevelWidth + j], (j * 12 - vp.x) * pge::draw::get_scale(), (i * 12 - vp.y) * pge::draw::get_scale());
                else
                    bgfiles->push_back(i * aLevelWidth + j);
            }

    for (int i = 0; i < files.size(); i++)
        draw_block(aLevel[files[i]], (files[i] % aLevelWidth * 12 - vp.x) * pge::draw::get_scale(), (files[i] / aLevelWidth * 12 - vp.y) * pge::draw::get_scale());
}

void EController::bc_render(pge::viewport::View& vp, std::vector<int>* bgfiles)
{
    std::vector<int> files;
    for (int i = 0; i < bgfiles->size(); i++) {
        if (aLevel[(*bgfiles)[i]]->type->type == bt_water)
            draw_block(aLevel[(*bgfiles)[i]], ((*bgfiles)[i] % aLevelWidth * 12 - vp.x) * pge::draw::get_scale(), ((*bgfiles)[i] / aLevelWidth * 12 - vp.y) * pge::draw::get_scale());
        else
            files.push_back((*bgfiles)[i]);
    }

    for (int i = 0; i < files.size(); i++)
        draw_block(aLevel[files[i]], (files[i] % aLevelWidth * 12 - vp.x) * pge::draw::get_scale(), (files[i] / aLevelWidth * 12 - vp.y) * pge::draw::get_scale());
}

void EController::bg_render(int ypos, pge::viewport::View& vp, int _time)
{
    SDL_Color color = lcontrol->get_skycolor();
    color.a = lcontrol->get_skyval() * 5.1;
    pge::image::set_colormod(bgsky, color);
    color.a = 255;

    env_draw_background_sky(nightsky, 0x5e7bd1ff, 0x5266ccff, 0);
    env_draw_background_sky(bgsky, 0x5e7bd1ff, 0x5266ccff, _time);

    for (int i = 0; i < 5; i++) {
        pge::image::set_colormod(bg[i], color);
    }

    env_draw_background(bg[4], 0.90, ypos, vp);
    env_draw_background(bg[3], 0.80, ypos, vp);
    env_draw_background(bg[2], 0.65, ypos, vp);
    env_draw_background(bg[1], 0.45, ypos, vp);
    env_draw_background(bg[0], 0.25, ypos, vp);
}

void sctBlock::tile(uint8_t tile)
{
    //no comment - checking performance
    corner = true;
    is0 = false;
    corner_ur = 0;
    corner_ul = 0;
    corner_br = 0;
    corner_bl = 0;
    side_u = nullptr;
    side_r = nullptr;
    side_l = nullptr;
    side_d = nullptr;

    bool tileA = tile & 1;
    tile >>= 1;
    uint8_t tileHFDB = tile & 1;
    tile >>= 1;
    bool tileC = tile & 1;
    tile >>= 1;
    tileHFDB += (tile & 1) * 0b10;
    tile >>= 1;
    bool tileE = tile & 1;
    tile >>= 1;
    tileHFDB += (tile & 1) * 0b100;
    tile >>= 1;
    bool tileG = tile & 1;
    tile >>= 1;
    tileHFDB += (tile & 1) * 0b1000;

    //Scheme 0xhgfedcba
    //e f g
    //d X h
    //c b a
    switch (tileHFDB) {
    //0:
    case 0b1111:
        clip = aBlock_getclip(0, rand() % 16);
        corner_ur = !tileG;
        corner_ul = !tileE;
        corner_br = !tileA;
        corner_bl = !tileC;
        break;
    //1:
    case 0b0000:
        corner = false;
        side_u = aBlock_getclip(17, rand() % 4);
        side_r = aBlock_getclip(19, rand() % 4);
        side_l = aBlock_getclip(18, rand() % 4);
        side_d = aBlock_getclip(20, rand() % 4);
        clip = aBlock_getclip(1, rand() % 16);
        break;
    //2:
    case 0b1010:
        corner = false;
        side_u = aBlock_getclip(17, rand() % 4);
        side_d = aBlock_getclip(20, rand() % 4);
        clip = aBlock_getclip(2, rand() % 16);
        break;
    //3:
    case 0b0101:
        corner = false;
        side_r = aBlock_getclip(19, rand() % 4);
        side_l = aBlock_getclip(18, rand() % 4);
        clip = aBlock_getclip(3, rand() % 16);
        break;
    //4:
    case 0b1011:
        corner_br = !tileA;
        corner_bl = !tileC;
        side_u = aBlock_getclip(17, rand() % 4);
        clip = aBlock_getclip(4, rand() % 16);
        break;
    //5:
    case 0b1101:
        corner_ur = !tileG;
        corner_br = !tileA;
        side_l = aBlock_getclip(18, rand() % 4);
        clip = aBlock_getclip(5, rand() % 16);
        break;
    //6:
    case 0b0111:
        corner_ul = !tileE;
        corner_bl = !tileC;
        side_r = aBlock_getclip(19, rand() % 4);
        clip = aBlock_getclip(6, rand() % 16);
        break;
    //7:
    case 0b1110:
        corner_ur = !tileG;
        corner_ul = !tileE;
        side_d = aBlock_getclip(20, rand() % 4);
        clip = aBlock_getclip(7, rand() % 16);
        break;
    //8:
    case 0b1000:
        corner = false;
        side_u = aBlock_getclip(17, rand() % 4);
        side_l = aBlock_getclip(18, rand() % 4);
        side_d = aBlock_getclip(20, rand() % 4);
        clip = aBlock_getclip(8, rand() % 16);
        break;
    //9:
    case 0b0010:
        corner = false;
        side_u = aBlock_getclip(17, rand() % 4);
        side_d = aBlock_getclip(20, rand() % 4);
        side_r = aBlock_getclip(19, rand() % 4);
        clip = aBlock_getclip(9, rand() % 16);
        break;
    //10:
    case 0b0001:
        corner = false;
        side_u = aBlock_getclip(17, rand() % 4);
        side_r = aBlock_getclip(19, rand() % 4);
        side_l = aBlock_getclip(18, rand() % 4);
        clip = aBlock_getclip(10, rand() % 16);
        break;
    //11:
    case 0b0100:
        corner = false;
        side_r = aBlock_getclip(19, rand() % 4);
        side_l = aBlock_getclip(18, rand() % 4);
        side_d = aBlock_getclip(20, rand() % 4);
        clip = aBlock_getclip(11, rand() % 16);
        break;
    //12:
    case 0b1001:
        corner_br = !tileA;
        side_u = aBlock_getclip(17, rand() % 4);
        side_l = aBlock_getclip(18, rand() % 4);
        clip = aBlock_getclip(12, rand() % 16);
        break;
    //13:
    case 0b0011:
        corner_bl = !tileC;
        side_u = aBlock_getclip(17, rand() % 4);
        side_r = aBlock_getclip(19, rand() % 4);
        clip = aBlock_getclip(13, rand() % 16);
        break;
    //14:
    case 0b1100:
        corner_ur = !tileG;
        side_l = aBlock_getclip(18, rand() % 4);
        side_d = aBlock_getclip(20, rand() % 4);
        clip = aBlock_getclip(14, rand() % 16);
        break;
    //15:
    case 0b0110:
        corner_ul = !tileE;
        side_d = aBlock_getclip(20, rand() % 4);
        side_r = aBlock_getclip(19, rand() % 4);
        clip = aBlock_getclip(15, rand() % 16);
        break;
    }
}

void sctBlock::tilebg(uint8_t tile, uint8_t empty)
{
    bcorner = true;
    bis0 = false;
    bcorner_ur = 0;
    bcorner_ul = 0;
    bcorner_br = 0;
    bcorner_bl = 0;
    bside_u = nullptr;
    bside_r = nullptr;
    bside_l = nullptr;
    bside_d = nullptr;

    bool tileA = tile & 1;
    tile >>= 1;
    uint8_t tileHFDB = tile & 1;
    tile >>= 1;
    bool tileC = tile & 1;
    tile >>= 1;
    tileHFDB += (tile & 1) * 0b10;
    tile >>= 1;
    bool tileE = tile & 1;
    tile >>= 1;
    tileHFDB += (tile & 1) * 0b100;
    tile >>= 1;
    bool tileG = tile & 1;
    tile >>= 1;
    tileHFDB += (tile & 1) * 0b1000;

    switch (tileHFDB) {
    //0:
    case 0b1111:
        bclip = aBlock_getclip(0, rand() % 16);
        bcorner_ur = !tileG;
        bcorner_ul = !tileE;
        bcorner_br = !tileA;
        bcorner_bl = !tileC;
        break;
    //1:
    case 0b0000:
        bcorner = false;
        bclip = aBlock_getclip(1, rand() % 16);
        bis0 = true;
        break;
    //2:
    case 0b1010:
        bcorner = false;
        bclip = aBlock_getclip(2, rand() % 16);
        break;
    //3:
    case 0b0101:
        bcorner = false;
        bclip = aBlock_getclip(3, rand() % 16);
        break;
    //4:
    case 0b1011:
        bcorner_br = !tileA;
        corner_bl = !tileC;
        bclip = aBlock_getclip(4, rand() % 16);
        break;
    //5:
    case 0b1101:
        bcorner_ur = !tileG;
        corner_br = !tileA;
        bclip = aBlock_getclip(5, rand() % 16);
        break;
    //6:
    case 0b0111:
        bcorner_ul = !tileE;
        bcorner_bl = !tileC;
        bclip = aBlock_getclip(6, rand() % 16);
        break;
    //7:
    case 0b1110:
        bcorner_ur = !tileG;
        bcorner_ul = !tileE;
        bclip = aBlock_getclip(7, rand() % 16);
        break;
    //8:
    case 0b1000:
        bcorner = false;
        bclip = aBlock_getclip(8, rand() % 16);
        break;
    //9:
    case 0b0010:
        corner = false;
        bclip = aBlock_getclip(9, rand() % 16);
        break;
    //10:
    case 0b0001:
        bcorner = false;
        bclip = aBlock_getclip(10, rand() % 16);
        break;
    //11:
    case 0b0100:
        bcorner = false;
        bclip = aBlock_getclip(11, rand() % 16);
        break;
    //12:
    case 0b1001:
        bcorner_br = !tileA;
        bclip = aBlock_getclip(12, rand() % 16);
        break;
    //13:
    case 0b0011:
        bcorner_bl = !tileC;
        bclip = aBlock_getclip(13, rand() % 16);
        break;
    //14:
    case 0b1100:
        bcorner_ur = !tileG;
        bclip = aBlock_getclip(14, rand() % 16);
        break;
    //15:
    case 0b0110:
        bcorner_ul = !tileE;
        bclip = aBlock_getclip(15, rand() % 16);
        break;
    }

    if (empty & 0b0001)
        bside_d = aBlock_getclip(20, rand() % 4);
    if (empty & 0b0010)
        bside_l = aBlock_getclip(18, rand() % 4);
    if (empty & 0b0100)
        bside_u = aBlock_getclip(17, rand() % 4);
    if (empty & 0b1000)
        bside_r = aBlock_getclip(19, rand() % 4);
}

//
bool EController::bkchk(uint32_t id)
{
    return (aLevel[id] != nullptr) && (aLevel[id]->type != nullptr);
}

uint8_t EController::bkchk_t(uint32_t id)
{
    if (aLevel[id] != nullptr) {
        return aLevel[id]->type->type;
    }
    return bt_empty;
}

bool EController::bkchk_m(uint32_t id, uint32_t id2)
{
    return (aLevel[id] != nullptr) && (aLevel[id]->type->match == aLevel[id2]->type->match);
}

bool EController::bgchk(uint32_t id)
{
    return (aLevel[id] != nullptr) && (aLevel[id]->back != nullptr);
}

bool EController::bgchk_m(uint32_t id, uint32_t id2)
{
    return (aLevel[id] != nullptr) && (aLevel[id]->back->match == aLevel[id2]->back->match);
}
