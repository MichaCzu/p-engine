#include "item.hpp"

std::map<en_itemID, Item> aItems = {
    { ii_apple, { ii_apple, ITEM_CONSUMEABLE,
                    et_mapply, et_mapply, 16, false,
                    "res/spr/hud/icons/apple.png",
                    { { ip_health, 8 } } } },
    { ii_steak, { ii_steak, ITEM_CONSUMEABLE,
                    et_mapply, et_mapply, 16, false,
                    "res/spr/hud/icons/steak.png",
                    { { ip_health, 32 } } } },
    { ii_sword, { ii_sword, ITEM_WEAPON1H,
                    et_mapply, et_mapply, 1, false,
                    "res/spr/hud/icons/sword.png",
                    {} } },
    { ii_shirt, { ii_shirt, ITEM_CHEST,
                    et_mapply, et_mapply, 1, false,
                    "res/spr/hud/icons/shirt.png",
                    { { ip_armor, 30 } } } },
    { ii_pants, { ii_pants, ITEM_LEGS,
                    et_mapply, et_mapply, 1, false,
                    "res/spr/hud/icons/pants.png",
                    { { ip_armor, 20 } } } },
    { ii_hat, { ii_hat, ITEM_HELMET,
                  et_mapply, et_mapply, 1, false,
                  "res/spr/hud/icons/hat.png",
                  { { ip_armor, 5 } } } },
    { ii_ringblue, { ii_ringblue, ITEM_RING,
                       et_mapply, et_mapply, 1, false,
                       "res/spr/hud/icons/ringblue.png",
                       { { ip_armor, 10 } } } },
    { ii_ringred, { ii_ringred, ITEM_RING,
                      et_mapply, et_mapply, 1, false,
                      "res/spr/hud/icons/ringred.png",
                      { { ip_armor, 2 } } } },
    { ii_amulet, { ii_amulet, ITEM_AMULET,
                     et_mapply, et_mapply, 1, false,
                     "res/spr/hud/icons/amulet.png",
                     { { ip_armor, 1 } } } },
    { ii_crystal, { ii_crystal, ITEM_ACTIVEABLE,
                      et_mapply, et_mapply, 1, false,
                      "res/spr/hud/icons/crystal.png",
                      { { ip_health, -16 }, { ip_health_req, 16 }, { ip_energy, 32 } } } },
};

bool ItemSlot::draw(float x, float y, uint16_t font, float scale, uint16_t gui)
{
    const pge::Rect rect = { 0, 0, 15, 15 };

    if (item != nullptr) {
        if (animation > animspeed)
            pge::image::draw(item->icon, x, y, &rect, NULL, 0, scale, scale);
        else {
            float transformX = -cos(float(animation) / float(animspeed) * 10) * (0.5 * (cos(float(animation) / animspeed * 3.14) + 1) / 2),
                  transformY = -transformX;
            pge::image::draw(item->icon, x - floor(15 * scale * transformX / 2), y - floor(15 * scale * transformY / 2), &rect, NULL, 0, scale * (1 + transformX), scale * (1 + transformY));
        }
        uint8_t alpha = pge::image::get_alpha(font);
        int digits = count < 10 ? 0 : count < 100 ? 1 : count < 1000 ? 2 : 3;
        int placehold = count;
        if (count > 1)
            for (int i = 0; i <= digits; i++) {
                float space = (3.66 * i - digits * 1) * (scale - 1);
                int number = placehold % 10;
                placehold = placehold / 10;
                pge::Rect nrect = { float(3 * number), 0, 3, 5 };
                pge::image::set_colormod(font, SDL_Color{ 0x14, 0x14, 0x28, alpha / 2 });
                pge::image::draw(font, x + (11.33) * scale - space, y + 10.5 * scale, &nrect, NULL, 0, scale - 1, scale - 1);

                pge::image::set_colormod(font, SDL_Color{ 255, 255, 255, alpha });
                pge::image::draw(font, x + (11) * scale - space, y + 10 * scale, &nrect, NULL, 0, scale - 1, scale - 1);
            }
        return true;
    } else {
        pge::Rect rectsloti;
        bool flag = true;
        switch (slottype) {
        default:
            flag = false;
            break;
        case ITEM_HELMET:
            rectsloti = pge::Rect(30, 46, 15, 15);
            break;
        case ITEM_CHEST:
            rectsloti = pge::Rect(45, 46, 15, 15);
            break;
        case ITEM_LEGS:
            rectsloti = pge::Rect(60, 46, 15, 15);
            break;
        case ITEM_RING:
            rectsloti = pge::Rect(75, 46, 15, 15);
            break;
        case ITEM_AMULET:
            rectsloti = pge::Rect(90, 46, 15, 15);
            break;
        case ITEM_MAGIC:
            rectsloti = pge::Rect(0, 46, 15, 15);
            break;
        case ITEM_SHIELD:
            rectsloti = pge::Rect(15, 46, 15, 15);
            break;
        }

        if (flag)
            pge::image::draw(gui, x - 1 * scale, y - 1 * scale, &rectsloti, NULL, 0, scale, scale);
        return false;
    }
}

//ItemProperty aItemProperties[] = {
//    { ip_health, et_mapply },
//};

//ItemProperty aItemType[] = {
//    { ip_health, et_mapply },
//};

bool aItems_Init()
{
    for (auto& _item : aItems) {
        if (_item.second.iconpath != "") {
            uint16_t flag = pge::image::load(_item.second.iconpath);
            if (flag < (uint16_t)(-1))
                _item.second.icon = flag;
            else
                return 0;
        }
    }
    return 1;
}

bool aItems_SetAlpha(uint8_t alpha)
{
    for (auto& _item : aItems) {
        if (_item.second.iconpath != "") {
            pge::image::set_colormod(_item.second.icon, SDL_Color{ 255, 255, 255, alpha });
        }
    }
    return 1;
}

bool ItemSlot::test_empty()
{
    if (item == nullptr)
        count = 0;
    if (count == 0)
        item = nullptr;

    return count > 0 ? count : 0;
}

uint16_t ItemSlot::test_quantity()
{
    if (item != nullptr)
        if (count > item->stacksize) {
            uint16_t flag = count - item->stacksize;
            count = item->stacksize;
            return flag;
        }
    return 0;
}

bool ItemSlot::switch_slots(ItemSlot* slot)
{
    slot->reanimate(true);

    if (slot->item == nullptr || !is_itemOk(slot->item))
        return false;
    reanimate(true);
    Item* itembuffer = item;
    uint16_t countbuffer = count;
    item = slot->item;
    count = slot->count;
    slot->item = itembuffer;
    slot->count = countbuffer;
    return true;
}

void ItemSlot::animate(uint16_t _time)
{
    if (animation < animspeed) {
        animation += _time;
    }
}

bool ItemSlot::is_itemOk(Item* _item)
{
    if (slottype == ITEM_MAGIC || slottype == ITEM_SHIELD || slottype == ITEM_GENERIC) {
        return true;
    } else {
        if (_item != nullptr && _item->type == slottype)
            return true;
        else
            return false;
    }
}
