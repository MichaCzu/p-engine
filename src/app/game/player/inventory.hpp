#ifndef PInventory_HPP
#define PInventory_HPP

#include "item.hpp"
#include "itemdef.hpp"
#include "pge/pge.hpp"

class Controller;

class PInventory {
    uint16_t gui;
    uint16_t numb;
    Controller* control;
    bool shown = false;
    ItemSlot inventory[24];
    bool expanded = false;
    ItemSlot helmet = ItemSlot(ITEM_HELMET);
    ItemSlot chest = ItemSlot(ITEM_CHEST);
    ItemSlot leggings = ItemSlot(ITEM_LEGS);
    ItemSlot ring1 = ItemSlot(ITEM_RING);
    ItemSlot amulet = ItemSlot(ITEM_AMULET);
    ItemSlot ring2 = ItemSlot(ITEM_RING);
    double transition = 0;

    int8_t selectedPrimary = 0;
    int8_t selectedSecondary = 1;
    int8_t selectionX = 0, selectionY = 0,
           selectedX, selectedY = -1;

    int16_t trash_emptying = 0;
    ItemSlot trash = ItemSlot(ITEM_SHIELD);

    const uint16_t button_delay = 166;
    bool s_up = false, s_down = false, s_left = false, s_right = false, s_super = false, s_shiftfor = false, s_shiftbac = false, s_selector = false, s_back = false;
    uint32_t t_up = 0, t_down = 0, t_left = 0, t_right = 0, t_super = 0, t_shiftfor = 0, t_shiftbac = 0;
    int get_pix();

protected:
    friend class Controller;
    friend class PController;

    PInventory(Controller* _control);
    ~PInventory();

    void draw_gui();
    void show(bool flag);
    bool is_shown() { return shown; }
    void handle_events();
    ItemSlot* get_slot(int8_t _x, int8_t _y);
    void update();
    void use(int8_t _x, int8_t _y);
    std::map<en_itemProperty, int> get_equipment();

public:
    uint16_t pickup_item(Item* _item, uint16_t _quantity); //return remaining items
};

#endif
