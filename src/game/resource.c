#include "resource.h"

#include "building/type.h"
#include "scenario/building.h"

#include <assert.h>

static const resource_type FOOD_RESOURCES[] = {
    RESOURCE_WHEAT, RESOURCE_VEGETABLES, RESOURCE_FRUIT, RESOURCE_MEAT
};

#define NUM_FOOD_RESOURCES (sizeof(FOOD_RESOURCES) / sizeof(resource_type))

static const resource_type GOOD_RESOURCES[] = {
    RESOURCE_OLIVES, RESOURCE_VINES, RESOURCE_WINE, RESOURCE_OIL, RESOURCE_IRON, RESOURCE_TIMBER,
    RESOURCE_CLAY, RESOURCE_MARBLE, RESOURCE_WEAPONS, RESOURCE_FURNITURE, RESOURCE_POTTERY
};

#define NUM_GOOD_RESOURCES (sizeof(GOOD_RESOURCES) / sizeof(resource_type))

static const resource_type SPECIAL_RESOURCES[] = {
    RESOURCE_DENARII, RESOURCE_TROOPS
};

#define NUM_SPECIAL_RESOURCES (sizeof(SPECIAL_RESOURCES) / sizeof(resource_type))

resource_type resource_get_food(int food_id)
{
    assert(food_id >= 0 && food_id < NUM_FOOD_RESOURCES);
    return FOOD_RESOURCES[food_id];
}

int resource_max_foods(void)
{
    return NUM_FOOD_RESOURCES;
}

resource_type resource_get_good(int good_id)
{
    return GOOD_RESOURCES[good_id];
}

int resource_max_goods(void)
{
    return NUM_GOOD_RESOURCES;
}

resource_type resource_special(int id)
{
    return SPECIAL_RESOURCES[id];
}

int resource_max_special(void)
{
    return NUM_SPECIAL_RESOURCES;
}


resource_type resource_get(int id)
{
    if (id < NUM_FOOD_RESOURCES) {
        return FOOD_RESOURCES[id];
    } else if (id < NUM_FOOD_RESOURCES + NUM_GOOD_RESOURCES) {
        return GOOD_RESOURCES[id - NUM_FOOD_RESOURCES];
    } else {
        return SPECIAL_RESOURCES[id - NUM_FOOD_RESOURCES - NUM_GOOD_RESOURCES];
    }
}

int resource_max(void)
{
    return NUM_FOOD_RESOURCES + NUM_GOOD_RESOURCES + NUM_SPECIAL_RESOURCES;
}

int resource_image_offset(resource_type resource, resource_image_type type)
{
    if (resource == RESOURCE_MEAT && scenario_building_allowed(BUILDING_WHARF)) {
        switch (type) {
            case RESOURCE_IMAGE_STORAGE: return 40;
            case RESOURCE_IMAGE_CART: return 648;
            case RESOURCE_IMAGE_FOOD_CART: return 8;
            case RESOURCE_IMAGE_ICON: return 11;
            default: return 0;
        }
    } else {
        return 0;
    }
}

int resource_is_food(resource_type resource)
{
    for (int i = 0; i < NUM_FOOD_RESOURCES; i++) {
        if (resource == FOOD_RESOURCES[i]) {
            return 1;
        }
    }
    return 0;
}

workshop_type resource_to_workshop_type(resource_type resource)
{
    switch (resource) {
        case RESOURCE_OLIVES:
            return WORKSHOP_OLIVES_TO_OIL;
        case RESOURCE_VINES:
            return WORKSHOP_VINES_TO_WINE;
        case RESOURCE_IRON:
            return WORKSHOP_IRON_TO_WEAPONS;
        case RESOURCE_TIMBER:
            return WORKSHOP_TIMBER_TO_FURNITURE;
        case RESOURCE_CLAY:
            return WORKSHOP_CLAY_TO_POTTERY;
        default:
            return WORKSHOP_NONE;
    }
}

int inventory_is_set(int inventory, int flag)
{
    return (inventory >> flag) & 1;
}

void inventory_set(int *inventory, int flag)
{
    *inventory |= 1 << flag; 
}

int resource_from_inventory(int inventory_id)
{
    switch (inventory_id) {
        case INVENTORY_WHEAT: return RESOURCE_WHEAT;
        case INVENTORY_VEGETABLES: return RESOURCE_VEGETABLES;
        case INVENTORY_FRUIT: return RESOURCE_FRUIT;
        case INVENTORY_MEAT: return RESOURCE_MEAT;
        case INVENTORY_POTTERY: return RESOURCE_POTTERY;
        case INVENTORY_FURNITURE: return RESOURCE_FURNITURE;
        case INVENTORY_OIL: return RESOURCE_OIL;
        case INVENTORY_WINE: return RESOURCE_WINE;
        default: return RESOURCE_NONE;
    } 
}

int resource_to_inventory(resource_type resource)
{
    switch (resource) {
        case RESOURCE_WHEAT: return INVENTORY_WHEAT;
        case RESOURCE_VEGETABLES: return INVENTORY_VEGETABLES;
        case RESOURCE_FRUIT: return INVENTORY_FRUIT;
        case RESOURCE_MEAT: return INVENTORY_MEAT;
        case RESOURCE_POTTERY: return INVENTORY_POTTERY;
        case RESOURCE_FURNITURE: return INVENTORY_FURNITURE;
        case RESOURCE_OIL: return INVENTORY_OIL;
        case RESOURCE_WINE: return INVENTORY_WINE;
        default: return INVENTORY_NONE;
    } 
}
