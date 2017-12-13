#ifndef ITEM_HPP
#define ITEM_HPP

#include "itemdef.hpp"
#include "pge/pge.hpp"
#include <map>

class Controller;

enum en_itemID {
    ii_empty = -1,
    ii_money = 0,
    ii_apple,
    ii_sword,
    ii_steak,
    ii_shirt,
    ii_pants,
    ii_hat,
    ii_ringred,
    ii_ringblue,
    ii_amulet,
    ii_crystal,
};

struct Item {
public:
    en_itemID id;
    en_itemType type;
    en_transl name;
    en_transl desc;
    int8_t stacksize;
    bool useable;
    std::string iconpath;
    std::map<en_itemProperty, int> properties;
    uint16_t icon = uint16_t(-1);

protected:
    friend class Controller;
};

//struct ItemCustom : public Item {
//
//};

struct ItemSlot {
public:
    Item* item = nullptr;
    //Modifier* mod = nullptr;
    bool isCustom = false;
    int16_t count = 1;
    int8_t damage = 0;
    uint16_t animation = 0;
    uint16_t animspeed = 800;
    en_itemType slottype;

    ItemSlot() { slottype = ITEM_GENERIC; }
    ItemSlot(en_itemType _slottype) { slottype = _slottype; }

    void animate(uint16_t _time);
    void reanimate(bool slow = false) { animation = slow ? 200 : 0; }
    bool is_itemOk(Item* _item);
    bool draw(float x, float y, uint16_t font, float scale, uint16_t gui);
    bool switch_slots(ItemSlot* slot);
    bool test_empty();
    bool test_itemtype();
    uint16_t test_quantity();
    //std::map<Parameter, get_stats()

protected:
    friend class Controller;
};

extern std::map<en_itemID, Item> aItems;
bool aItems_Init();
//bool aItems_SetAlpha(Item item, uint8_t alpha);
bool aItems_SetAlpha(uint8_t alpha);

#endif
