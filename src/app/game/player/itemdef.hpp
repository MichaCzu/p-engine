#ifndef ITEMDEF_HPP
#define ITEMDEF_HPP

#include "pge/pge.hpp"
#include <map>

//
//////////
//

enum en_itemType {
    ITEM_WEAPON2H = 0,
    ITEM_WEAPON1H,
    ITEM_PRJWEP1H,
    ITEM_PRJWEP2H,
    ITEM_MAGIC,
    ITEM_SHIELD,

    ITEM_HELMET = 20,
    ITEM_CHEST = 21,
    ITEM_LEGS = 22,
    ITEM_RING = 23,
    ITEM_AMULET = 24,

    ITEM_CURRENCY = 100,
    ITEM_GENERIC,
    ITEM_CONSUMEABLE,
    ITEM_ACTIVEABLE,
};

struct ItemType {
    en_itemType id;
    en_transl name;
};

//
//////////
//

enum en_itemProperty {
    ip_health,
    ip_health_req,
    ip_energy,
    ip_energy_req,
    ip_armor,
};

struct ItemProperty {
    en_itemProperty id;
    en_transl name;
};

//
//////////
//

#endif
