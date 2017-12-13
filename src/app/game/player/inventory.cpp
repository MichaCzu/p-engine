#include "inventory.hpp"
#include "app/game/controller.hpp"
#include "app/game/player_controller.hpp"

PInventory::PInventory(Controller* _control)
{
    control = _control;
    for (int i = 0; i < 6; i++) {
        inventory[i].slottype = ITEM_MAGIC;
    }

    gui = control->res()->get_image(401, "res/spr/hud/gui/gui.png", 0xFF00FFFF);
    numb = control->res()->get_image(403, "res/spr/hud/gui/numb.png", 0xFF00FFFF);
    pickup_item(&aItems[ii_sword], 1);
    pickup_item(&aItems[ii_apple], 25);
    pickup_item(&aItems[ii_steak], 12);
    pickup_item(&aItems[ii_apple], 11);
}

PInventory::~PInventory()
{
}

void PInventory::handle_events()
{
    static bool changed = false;
    if (s_shiftbac) {
        if (t_shiftbac < pge::get_time_passed()) {
            t_shiftbac = pge::get_time_passed() + button_delay;
            int8_t* selected = s_super ? &selectedSecondary : &selectedPrimary;
            (*selected)--;
            if (*selected < 0)
                *selected = 5;
            if (selectedPrimary == selectedSecondary)
                (*selected)--;
            if (*selected < 0)
                *selected = 5;
        }
    } else
        t_shiftbac = 0;

    if (s_shiftfor) {
        if (t_shiftfor < pge::get_time_passed()) {
            t_shiftfor = pge::get_time_passed() + button_delay;
            int8_t* selected = s_super ? &selectedSecondary : &selectedPrimary;
            (*selected)++;
            if (*selected > 5)
                *selected = 0;
            if (selectedPrimary == selectedSecondary)
                (*selected)++;
            if (*selected > 5)
                *selected = 0;
        }
    } else
        t_shiftfor = 0;

    if (shown) {
        if (s_up) {
            if (t_up < pge::get_time_passed()) {
                t_up = pge::get_time_passed() + button_delay;
                selectionY++;
                changed = true;
            }
        } else
            t_up = 0;
        if (s_down) {
            if (t_down < pge::get_time_passed()) {
                t_down = pge::get_time_passed() + button_delay;
                selectionY--;
                if (selectionY == 0 && selectionX == -1)
                    selectionY = 3;
                if (selectionY == -1 && selectionX == 6)
                    selectionY = 3;
                changed = true;
            }
        } else
            t_down = 0;
        if (s_left) {
            if (t_left < pge::get_time_passed()) {
                t_left = pge::get_time_passed() + button_delay;
                selectionX--;
                if (selectionY == 0 && selectionX == -1)
                    selectionX = 6;
                if (!expanded && selectionY == 3 && selectionX == 5)
                    selectionY = 2;
                changed = true;
            }
        } else
            t_left = 0;
        if (s_right) {
            if (t_right < pge::get_time_passed()) {
                t_right = pge::get_time_passed() + button_delay;
                selectionX++;
                if (!expanded && selectionX == 0 && selectionY == 3)
                    selectionY = 2;
                if (selectionY == 0 && selectionX == 7)
                    selectionX = 0;
                changed = true;
            }
        } else
            t_right = 0;

        if (s_selector) {
            s_selector = false;
            if (selectedY == -1 && get_slot(selectionX, selectionY)->item != nullptr) {
                selectedX = selectionX;
                selectedY = selectionY;
            } else if (selectedY != -1) {
                if (selectedX == selectionX && selectedY == selectionY) {
                    use(selectionX, selectionY);
                } else {
                    ItemSlot* slot = get_slot(selectionX, selectionY);
                    ItemSlot* slot2 = get_slot(selectedX, selectedY);
                    if (selectionX == 6 && selectionY == 0) {
                        slot->item = slot2->item;
                        slot->count = slot2->count;
                        slot2->count = 0;
                        slot->reanimate(true);
                        slot2->item = nullptr;
                        slot2->count = 0;
                    } else if (slot->item == slot2->item) {
                        if (slot->count + slot2->count <= slot->item->stacksize) {
                            slot->count += slot2->count;
                            slot->reanimate(true);
                            slot2->item = nullptr;
                            slot2->count = 0;
                        } else if (slot->count < slot->item->stacksize) {
                            int8_t transfer = (slot->count + slot2->count) % slot->item->stacksize;
                            slot->count = slot->item->stacksize;
                            slot->reanimate(true);
                            slot2->reanimate(true);
                            slot2->count = transfer;
                        } else {
                            slot->switch_slots(slot2);
                            update();
                        }
                    } else {
                        slot->switch_slots(slot2);
                        update();
                    }
                }

                selectedY = -1;
            }
        }

        if (s_back) {
            s_back = false;
            if (selectedY != -1)
                selectedY = -1;
            else
                show(false);
        }

        if (changed) {
            changed = false;
            if (selectionX > 6)
                selectionX = -1;

            if (selectionX < -1)
                selectionX = 6;

            if (selectionY < 0)
                selectionY = 2 + expanded;

            if ((selectionX >= 0 && selectionX < 6 && selectionY > 2 + expanded) || selectionY > 3)
                selectionY = selectionX == -1 ? 1 : 0;
        }
    }

    if (!shown || (trash.item != nullptr && (selectionX != 6 || selectionY != 0) && (selectedX != 6 || selectedY != 0))) {
        trash_emptying += pge::get_time_difference();
        if (trash_emptying > 10000) {
            trash_emptying = 0;
            trash.item = nullptr;
            trash.count = 0;
        }
    } else if (trash_emptying != 0)
        trash_emptying = 0;

    for (int i = 0; i < 24; i++)
        inventory[i].animate(pge::get_time_difference());
    helmet.animate(pge::get_time_difference());
    chest.animate(pge::get_time_difference());
    leggings.animate(pge::get_time_difference());
    ring2.animate(pge::get_time_difference());
    ring1.animate(pge::get_time_difference());
    amulet.animate(pge::get_time_difference());
    trash.animate(pge::get_time_difference());
}

void PInventory::draw_gui()
{
    int scale = pge::draw::get_scale() + 1;
    int w, h;
    pge::window::get_size(&w, &h);
    pge::Rect rectslotb(0, 0, 18, 18);
    pge::Rect rectsloti(0, 0, 18, 18);
    pge::Rect rectslotsld(73 + (pge::get_time_passed() / 500) % 2 * 17, 18, 17, 17);
    pge::Rect rectslotsel(56, 18, 17, 17);
    bool guiselector = true;
    bool guiselected = true;

    if (shown && transition != 20) {
        transition += ((20 - transition) / 200 + 0.001) * pge::get_time_difference();
        if (transition > 20)
            transition = 20;
    } else if (!shown && transition != 0.0) {
        transition -= ((transition) / 200 + 0.001) * pge::get_time_difference();
        if (transition < 0)
            transition = 0;
    }

    if (transition > 0) {
        float var = 20 - transition;
        pge::draw::rectFill(0, 0, w, h, SDL_Color{ 0, 0, 0, (1 - var / 20) * 150 });
        pge::image::set_colormod(gui, SDL_Color{ 0xFF, 0xFF, 0xFF, (1 - var / 20) * 255 });
        pge::image::set_colormod(numb, SDL_Color{ 0xFF, 0xFF, 0xFF, (1 - var / 20) * 255 });
        aItems_SetAlpha((1 - var / 20) * 255);

        //draw slots
        //inventory
        int rows = expanded ? 3 : 2;
        rectslotb = pge::Rect(0, 0, 18, 18);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < 6; j++) {
                pge::image::draw(gui, w / 2 - ((57 - 19 * j) * scale), h - 16 - (46 - var + i * 19) * scale, &rectslotb, NULL, 0, scale, scale);
            }
        //item helmet
        rectslotb = pge::Rect(54, 0, 18, 18);
        pge::image::draw(gui, w / 2 - (76) * scale, h - 16 - (76 - var) * scale, &rectslotb, NULL, 0, scale, scale);
        //item chestplate
        pge::image::draw(gui, w / 2 - (76) * scale, h - 16 - (57 - var) * scale, &rectslotb, NULL, 0, scale, scale);
        //item leggings
        pge::image::draw(gui, w / 2 - (76) * scale, h - 16 - (38 - var) * scale, &rectslotb, NULL, 0, scale, scale);
        //item ring
        pge::image::draw(gui, w / 2 + (57) * scale, h - 16 - (76 - var) * scale, &rectslotb, NULL, 0, scale, scale);
        pge::image::draw(gui, w / 2 + (57) * scale, h - 16 - (38 - var) * scale, &rectslotb, NULL, 0, scale, scale);
        //item amulet
        pge::image::draw(gui, w / 2 + (57) * scale, h - 16 - (57 - var) * scale, &rectslotb, NULL, 0, scale, scale);
        //trash
        rectslotb = pge::Rect(72, 0, 18, 18);
        pge::image::draw(gui, w / 2 + (57) * scale, h - 16 - (19 - var) * scale, &rectslotb, NULL, 0, scale, scale);
        //draw selector
        if (selectionX == -1 || selectionX == 6 || selectionX == 6)
            pge::image::draw(gui, w / 2 - ((57 - 19 * selectionX) * scale), h - 16 - (19 - var + selectionY * 19) * scale, &rectslotsel, NULL, 0, scale, scale);
        else if (selectionY != 0)
            pge::image::draw(gui, w / 2 - ((57 - 19 * selectionX) * scale), h - 16 - (27 - var + selectionY * 19) * scale, &rectslotsel, NULL, 0, scale, scale);
        else
            guiselector = false;

        if (selectedY >= 0 && (selectedX != selectionX || selectedY != selectionY)) {
            if (selectedX == -1 || selectedX == 6 || selectedX == 6)
                pge::image::draw(gui, w / 2 - ((57 - 19 * selectedX) * scale), h - 16 - (19 - var + selectedY * 19) * scale, &rectslotsld, NULL, 0, scale, scale);
            else if (selectedY != 0)
                pge::image::draw(gui, w / 2 - ((57 - 19 * selectedX) * scale), h - 16 - (27 - var + selectedY * 19) * scale, &rectslotsld, NULL, 0, scale, scale);
            else
                guiselected = false;
        }
        //draw items
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < 6; j++)
                inventory[j + 6 * (i + 1)].draw(w / 2 - ((56 - 19 * j) * scale), h - 16 - (45 - var + i * 19) * scale, numb, scale, gui);
        helmet.draw(w / 2 - (75) * scale, h - 16 - (75 - var) * scale, numb, scale, gui);
        chest.draw(w / 2 - (75) * scale, h - 16 - (56 - var) * scale, numb, scale, gui);
        leggings.draw(w / 2 - (75) * scale, h - 16 - (37 - var) * scale, numb, scale, gui);
        ring1.draw(w / 2 + (58) * scale, h - 16 - (75 - var) * scale, numb, scale, gui);
        ring2.draw(w / 2 + (58) * scale, h - 16 - (37 - var) * scale, numb, scale, gui);
        amulet.draw(w / 2 + (58) * scale, h - 16 - (56 - var) * scale, numb, scale, gui);
        trash.draw(w / 2 + (58) * scale, h - 16 - (18 - var) * scale, numb, scale, gui);

        if (trash_emptying > 0) {
            rectsloti = pge::Rect(56, 35 - (trash_emptying)*17 / 10000, 17, (trash_emptying)*17 / 10000);
            pge::image::draw(gui, w / 2 + (57) * scale, h - 16 - (2 - var + (trash_emptying)*17 / 10000) * scale, &rectsloti, NULL, 0, scale, scale);
        }

        pge::image::set_colormod(gui, SDL_Color{ 255, 255, 255, 255 });
        pge::image::set_colormod(numb, SDL_Color{ 255, 255, 255, 255 });
        aItems_SetAlpha(255);
    }

    for (int i = 0; i < 6; i++) {
        pge::Rect rect;
        if (i == selectedPrimary)
            rect = pge::Rect(18, 0, 18, 18);
        else if (i == selectedSecondary)
            rect = pge::Rect(36, 0, 18, 18);
        else
            rect = pge::Rect(0, 0, 18, 18);
        pge::image::draw(gui, w / 2 - ((57 - 19 * i) * scale), h - 16 - 19 * scale, &rect, NULL, 0, scale, scale);
    }

    //drawing selection
    if (transition > 0 && !guiselector) {
        pge::image::set_colormod(gui, SDL_Color{ 255, 255, 255, (1 - (20 - transition) / 20) * 255 });
        pge::image::draw(gui, w / 2 - ((57 - 19 * selectionX) * scale), h - 16 - (19) * scale, &rectslotsel, NULL, 0, scale, scale);
        pge::image::set_colormod(gui, SDL_Color{ 255, 255, 255, 255 });
    }
    if (transition > 0 && !guiselected && selectedY >= 0 && (selectionX != selectedX || selectionY != selectedY)) {
        pge::image::set_colormod(gui, SDL_Color{ 255, 255, 255, (1 - (20 - transition) / 20) * 255 });
        pge::image::draw(gui, w / 2 - ((57 - 19 * selectedX) * scale), h - 16 - (19) * scale, &rectslotsld, NULL, 0, scale, scale);
        pge::image::set_colormod(gui, SDL_Color{ 255, 255, 255, 255 });
    }

    rectsloti = pge::Rect(0, 46, 15, 15);
    for (int i = 0; i < 6; i++)
        inventory[i].draw(w / 2 - ((56 - 19 * i) * scale), h - 16 - 18 * scale, numb, scale, gui);
}

void PInventory::show(bool flag)
{
    if (shown ? true : transition == 0) {
        control->set_motion(flag ? 0 : 1);
        shown = flag;
        selectedY = -1;
    }
}

ItemSlot* PInventory::get_slot(int8_t _x, int8_t _y)
{
    if (_y >= 0 && _y <= 3) {
        if (_x == -1) {
            if (_y == 1)
                return &leggings;
            else if (_y == 2)
                return &chest;
            else if (_y == 3)
                return &helmet;
        } else if (_x >= 0 && _x < 6)
            return &inventory[_x + 6 * _y];
        else if (_x == 6) {
            if (_y == 0)
                return &trash;
            else if (_y == 1)
                return &ring2;
            else if (_y == 2)
                return &amulet;
            else if (_y == 3)
                return &ring1;
        }
    }
    return nullptr;
}

void PInventory::update()
{
    //testing removed

    //testing equipment slots
    if (!expanded)
        for (int i = 18; i < 18; i++) {
            if (inventory[i].item != nullptr) {
                Item* buffitem = inventory[i].item;
                uint16_t buffcount = inventory[i].count;
                inventory[i].item = nullptr;
                pickup_item(buffitem, buffcount);
            }
        }

    if (helmet.item != nullptr && helmet.item->type != ITEM_HELMET) {
        Item* buffitem = helmet.item;
        uint16_t buffcount = helmet.count;
        helmet.item = nullptr;
        pickup_item(buffitem, buffcount);
    }
    if (chest.item != nullptr && chest.item->type != ITEM_CHEST) {
        Item* buffitem = chest.item;
        uint16_t buffcount = chest.count;
        chest.item = nullptr;
        pickup_item(buffitem, buffcount);
    }
    if (leggings.item != nullptr && leggings.item->type != ITEM_LEGS) {
        Item* buffitem = leggings.item;
        uint16_t buffcount = leggings.count;
        leggings.item = nullptr;
        pickup_item(buffitem, buffcount);
    }
    if (ring1.item != nullptr && ring1.item->type != ITEM_RING) {
        Item* buffitem = ring1.item;
        uint16_t buffcount = ring1.count;
        ring1.item = nullptr;
        pickup_item(buffitem, buffcount);
    }
    if (ring2.item != nullptr && ring2.item->type != ITEM_RING) {
        Item* buffitem = ring2.item;
        uint16_t buffcount = ring2.count;
        ring2.item = nullptr;
        pickup_item(buffitem, buffcount);
    }
    if (amulet.item != nullptr && amulet.item->type != ITEM_AMULET) {
        Item* buffitem = amulet.item;
        uint16_t buffcount = amulet.count;
        amulet.item = nullptr;
        pickup_item(buffitem, buffcount);
    }

    for (int i = 0; i < 24; i++)
        inventory[i].test_empty();
    helmet.test_empty();
    chest.test_empty();
    leggings.test_empty();
    ring1.test_empty();
    ring2.test_empty();
    amulet.test_empty();

    //testing stacksize
    uint16_t transfer;
    for (int i = 0; i < 24; i++) {
        transfer = inventory[i].test_quantity();
        if (transfer > 0)
            pickup_item(inventory[i].item, transfer);
    }
    transfer = helmet.test_quantity();
    if (transfer > 0)
        pickup_item(helmet.item, transfer);

    transfer = chest.test_quantity();
    if (transfer > 0)
        pickup_item(chest.item, transfer);

    transfer = leggings.test_quantity();
    if (transfer > 0)
        pickup_item(leggings.item, transfer);

    transfer = ring1.test_quantity();
    if (transfer > 0)
        pickup_item(ring1.item, transfer);

    transfer = ring2.test_quantity();
    if (transfer > 0)
        pickup_item(ring2.item, transfer);

    transfer = amulet.test_quantity();
    if (transfer > 0)
        pickup_item(amulet.item, transfer);
}

uint16_t PInventory::pickup_item(Item* _item, uint16_t _quantity)
{
    if (_item == nullptr || _quantity <= 0)
        return true;

    switch (_item->type) {
    case ITEM_HELMET:
        if (helmet.item == nullptr) {
            helmet.item = _item;
            helmet.count = 1;
            helmet.reanimate(1);
            _quantity--;
        }
        break;
    case ITEM_CHEST:
        if (chest.item == nullptr) {
            chest.item = _item;
            chest.count = 1;
            chest.reanimate(1);
            _quantity--;
        }
        break;
    case ITEM_LEGS:
        if (leggings.item == nullptr) {
            leggings.item = _item;
            leggings.count = 1;
            leggings.reanimate(1);
            _quantity--;
        }

        break;
    case ITEM_RING:
        if (ring1.item == nullptr) {
            ring1.item = _item;
            ring1.count = 1;
            ring1.reanimate(1);
            _quantity--;
        } else if (ring2.item == nullptr) {
            ring2.item = _item;
            ring2.count = 1;
            ring2.reanimate(1);
            _quantity--;
        }

        break;
    case ITEM_AMULET:
        if (amulet.item == nullptr) {
            amulet.item = _item;
            amulet.count = 1;
            amulet.reanimate(1);
            _quantity--;
        }
        break;
    default:
        break;
    }
    get_equipment();

    for (int i = 0; i < 18 + 6 * expanded && _quantity > 0; i++) {
        if (_item == inventory[i].item) {
            if (inventory[i].count + _quantity < inventory[i].item->stacksize) {
                inventory[i].reanimate();
                inventory[i].count += _quantity;
                _quantity = 0;
            } else if (inventory[i].count != inventory[i].item->stacksize) {
                _quantity -= inventory[i].item->stacksize - inventory[i].count;
                inventory[i].count = inventory[i].item->stacksize;
                inventory[i].reanimate();
            }
        }
    }

    for (int i = 0; i < 18 + 6 * expanded && _quantity > 0; i++) {
        if (inventory[i].item == nullptr) {
            inventory[i].item = _item;
            if (_quantity <= inventory[i].item->stacksize) {
                inventory[i].reanimate();
                inventory[i].count = _quantity;
                _quantity = 0;
            } else {
                _quantity -= inventory[i].item->stacksize;
                inventory[i].count = inventory[i].item->stacksize;
                inventory[i].reanimate();
            }
        }
    }

    //while splitting quantity to stacksize
    //for stacks picked up
    //if equipment type and slot empty - equip
    //else searching for first free slot
    //if no empty slot refuse pickup
    update();
    return _quantity > 0 ? _quantity : 0;
}

void PInventory::use(int8_t _x, int8_t _y)
{
    ItemSlot* slot = get_slot(_x, _y);

    if (!control->plr()->check_item_requirements(slot->item))
        return;

    switch (slot->item->type) {
    default:
        break;
    case ITEM_CONSUMEABLE:
        slot->count--;
    case ITEM_ACTIVEABLE:
        control->plr()->use_item(slot->item);
        slot->reanimate();
        break;
    case ITEM_HELMET:
        helmet.switch_slots(slot);
        break;
    case ITEM_CHEST:
        chest.switch_slots(slot);
        break;
    case ITEM_LEGS:
        leggings.switch_slots(slot);
        break;
    case ITEM_RING:
        if (ring1.item == nullptr)
            ring1.switch_slots(slot);
        else if (ring2.item == nullptr)
            ring2.switch_slots(slot);
        else
            ring1.switch_slots(slot);
        break;
    case ITEM_AMULET:
        amulet.switch_slots(slot);
        break;
    }

    update();
}

std::map<en_itemProperty, int> PInventory::get_equipment()
{
    std::map<en_itemProperty, int> flag;
    if (helmet.item != nullptr)
        for (auto& _property : helmet.item->properties)
            if (flag[_property.first])
                flag[_property.first] += _property.second;
            else
                flag[_property.first] = _property.second;

    if (chest.item != nullptr)
        for (auto& _property : chest.item->properties)
            if (flag[_property.first])
                flag[_property.first] += _property.second;
            else
                flag[_property.first] = _property.second;

    if (leggings.item != nullptr)
        for (auto& _property : leggings.item->properties)
            if (flag[_property.first])
                flag[_property.first] += _property.second;
            else
                flag[_property.first] = _property.second;

    if (ring1.item != nullptr)
        for (auto& _property : ring1.item->properties)
            if (flag[_property.first])
                flag[_property.first] += _property.second;
            else
                flag[_property.first] = _property.second;

    if (ring2.item != nullptr)
        for (auto& _property : ring2.item->properties)
            if (flag[_property.first])
                flag[_property.first] += _property.second;
            else
                flag[_property.first] = _property.second;

    if (amulet.item != nullptr)
        for (auto& _property : amulet.item->properties)
            if (flag[_property.first])
                flag[_property.first] += _property.second;
            else
                flag[_property.first] = _property.second;

    pge::debug::log(std::to_string(flag[ip_armor]));
    return flag;
}
