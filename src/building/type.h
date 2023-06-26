#ifndef BUILDING_TYPE_H
#define BUILDING_TYPE_H

/**
 * @file
 * Type definitions for buildings
 */

 /**
  * Building types
  */
typedef enum {
    BUILDING_NONE = 0,
    BUILDING_MENU_FARMS = 2,
    BUILDING_MENU_RAW_MATERIALS = 3,
    BUILDING_MENU_WORKSHOPS = 4,
    BUILDING_ROAD = 5,
    BUILDING_WALL = 6,
    BUILDING_DRAGGABLE_RESERVOIR = 7,
    BUILDING_AQUEDUCT = 8,
    BUILDING_CLEAR_LAND = 9,
    BUILDING_HOUSE_VACANT_LOT = 10,
    BUILDING_HOUSE_SMALL_TENT = 10,
    BUILDING_HOUSE_LARGE_TENT = 11,
    BUILDING_HOUSE_SMALL_SHACK = 12,
    BUILDING_HOUSE_LARGE_SHACK = 13,
    BUILDING_HOUSE_SMALL_HOVEL = 14,
    BUILDING_HOUSE_LARGE_HOVEL = 15,
    BUILDING_HOUSE_SMALL_CASA = 16,
    BUILDING_HOUSE_LARGE_CASA = 17,
    BUILDING_HOUSE_SMALL_INSULA = 18,
    BUILDING_HOUSE_MEDIUM_INSULA = 19,
    BUILDING_HOUSE_LARGE_INSULA = 20,
    BUILDING_HOUSE_GRAND_INSULA = 21,
    BUILDING_HOUSE_SMALL_VILLA = 22,
    BUILDING_HOUSE_MEDIUM_VILLA = 23,
    BUILDING_HOUSE_LARGE_VILLA = 24,
    BUILDING_HOUSE_GRAND_VILLA = 25,
    BUILDING_HOUSE_SMALL_PALACE = 26,
    BUILDING_HOUSE_MEDIUM_PALACE = 27,
    BUILDING_HOUSE_LARGE_PALACE = 28,
    BUILDING_HOUSE_LUXURY_PALACE = 29,
    BUILDING_AMPHITHEATER = 30,
    BUILDING_THEATER = 31,
    BUILDING_HIPPODROME = 32,
    BUILDING_COLOSSEUM = 33,
    BUILDING_GLADIATOR_SCHOOL = 34,
    BUILDING_LION_HOUSE = 35,
    BUILDING_ACTOR_COLONY = 36,
    BUILDING_CHARIOT_MAKER = 37,
    BUILDING_PLAZA = 38,
    BUILDING_GARDENS = 39,
    BUILDING_FORT_LEGIONARIES = 40,
    BUILDING_SMALL_STATUE = 41,
    BUILDING_MEDIUM_STATUE = 42,
    BUILDING_LARGE_STATUE = 43,
    BUILDING_FORT_JAVELIN = 44,
    BUILDING_FORT_MOUNTED = 45,
    BUILDING_DOCTOR = 46,
    BUILDING_HOSPITAL = 47,
    BUILDING_BATHHOUSE = 48,
    BUILDING_BARBER = 49,
    BUILDING_DISTRIBUTION_CENTER_UNUSED = 50,
    BUILDING_SCHOOL = 51,
    BUILDING_ACADEMY = 52,
    BUILDING_LIBRARY = 53,
    BUILDING_FORT_GROUND = 54,
    BUILDING_PREFECTURE = 55,
    BUILDING_TRIUMPHAL_ARCH = 56,
    BUILDING_FORT = 57,
    BUILDING_GATEHOUSE = 58,
    BUILDING_TOWER = 59,
    BUILDING_SMALL_TEMPLE_CERES = 60,
    BUILDING_SMALL_TEMPLE_NEPTUNE = 61,
    BUILDING_SMALL_TEMPLE_MERCURY = 62,
    BUILDING_SMALL_TEMPLE_MARS = 63,
    BUILDING_SMALL_TEMPLE_VENUS = 64,
    BUILDING_LARGE_TEMPLE_CERES = 65,
    BUILDING_LARGE_TEMPLE_NEPTUNE = 66,
    BUILDING_LARGE_TEMPLE_MERCURY = 67,
    BUILDING_LARGE_TEMPLE_MARS = 68,
    BUILDING_LARGE_TEMPLE_VENUS = 69,
    BUILDING_MARKET = 70,
    BUILDING_GRANARY = 71,
    BUILDING_WAREHOUSE = 72,
    BUILDING_WAREHOUSE_SPACE = 73,
    BUILDING_SHIPYARD = 74,
    BUILDING_DOCK = 75,
    BUILDING_WHARF = 76,
    BUILDING_GOVERNORS_HOUSE = 77,
    BUILDING_GOVERNORS_VILLA = 78,
    BUILDING_GOVERNORS_PALACE = 79,
    BUILDING_MISSION_POST = 80,
    BUILDING_ENGINEERS_POST = 81,
    BUILDING_LOW_BRIDGE = 82,
    BUILDING_SHIP_BRIDGE = 83,
    BUILDING_SENATE_1_UNUSED = 84, // listed as "Senate 1" in the model.txt
    BUILDING_SENATE = 85,
    BUILDING_FORUM = 86,
    BUILDING_FORUM_2_UNUSED = 87, // listed as "Forum 2" in the model.txt
    BUILDING_NATIVE_HUT = 88,
    BUILDING_NATIVE_MEETING = 89,
    BUILDING_RESERVOIR = 90,
    BUILDING_FOUNTAIN = 91,
    BUILDING_WELL = 92,
    BUILDING_NATIVE_CROPS = 93,
    BUILDING_MILITARY_ACADEMY = 94,
    BUILDING_BARRACKS = 95,
    BUILDING_MENU_SMALL_TEMPLES = 96,
    BUILDING_MENU_LARGE_TEMPLES = 97,
    BUILDING_ORACLE = 98,
    BUILDING_BURNING_RUIN = 99,
    BUILDING_WHEAT_FARM = 100,
    BUILDING_VEGETABLE_FARM = 101,
    BUILDING_FRUIT_FARM = 102,
    BUILDING_OLIVE_FARM = 103,
    BUILDING_VINES_FARM = 104,
    BUILDING_PIG_FARM = 105,
    BUILDING_MARBLE_QUARRY = 106,
    BUILDING_IRON_MINE = 107,
    BUILDING_TIMBER_YARD = 108,
    BUILDING_CLAY_PIT = 109,
    BUILDING_WINE_WORKSHOP = 110,
    BUILDING_OIL_WORKSHOP = 111,
    BUILDING_WEAPONS_WORKSHOP = 112,
    BUILDING_FURNITURE_WORKSHOP = 113,
    BUILDING_POTTERY_WORKSHOP = 114,
    BUILDING_ROADBLOCK = 115,
    BUILDING_WORKCAMP = 116,
    BUILDING_GRAND_TEMPLE_CERES = 117,
    BUILDING_GRAND_TEMPLE_NEPTUNE = 118,
    BUILDING_GRAND_TEMPLE_MERCURY = 119,
    BUILDING_GRAND_TEMPLE_MARS = 120,
    BUILDING_GRAND_TEMPLE_VENUS = 121,
    BUILDING_MENU_GRAND_TEMPLES = 122,
    BUILDING_MENU_TREES = 123,
    BUILDING_MENU_PATHS = 124,
    BUILDING_MENU_PARKS = 125,
    BUILDING_SMALL_POND = 126,
    BUILDING_LARGE_POND = 127,
    BUILDING_PINE_TREE = 128,
    BUILDING_FIR_TREE = 129,
    BUILDING_OAK_TREE = 130,
    BUILDING_ELM_TREE = 131,
    BUILDING_FIG_TREE = 132,
    BUILDING_PLUM_TREE = 133,
    BUILDING_PALM_TREE = 134,
    BUILDING_DATE_TREE = 135,
    BUILDING_PINE_PATH = 136,
    BUILDING_FIR_PATH = 137,
    BUILDING_OAK_PATH = 138,
    BUILDING_ELM_PATH = 139,
    BUILDING_FIG_PATH = 140,
    BUILDING_PLUM_PATH = 141,
    BUILDING_PALM_PATH = 142,
    BUILDING_DATE_PATH = 143,
    BUILDING_PAVILION_BLUE = 144,
    BUILDING_PAVILION_RED = 145,
    BUILDING_PAVILION_ORANGE = 146,
    BUILDING_PAVILION_YELLOW = 147,
    BUILDING_PAVILION_GREEN = 148,
    BUILDING_SMALL_STATUE_ALT = 149,
    BUILDING_SMALL_STATUE_ALT_B = 150,
    BUILDING_OBELISK = 151,
    BUILDING_PANTHEON = 152,
    BUILDING_ARCHITECT_GUILD = 153,
    BUILDING_MESS_HALL = 154,
    BUILDING_LIGHTHOUSE = 155,
    BUILDING_MENU_STATUES = 156,
    BUILDING_MENU_GOV_RES = 157,
    BUILDING_TAVERN = 158,
    BUILDING_GRAND_GARDEN = 159,
    BUILDING_ARENA = 160,
    BUILDING_HORSE_STATUE = 161,
    BUILDING_DOLPHIN_FOUNTAIN = 162,
    BUILDING_HEDGE_DARK = 163,
    BUILDING_HEDGE_LIGHT = 164,
    BUILDING_LOOPED_GARDEN_WALL = 165,
    BUILDING_LEGION_STATUE = 166,
    BUILDING_DECORATIVE_COLUMN = 167,
    BUILDING_COLONNADE = 168,
    BUILDING_LARARIUM = 169,
    BUILDING_NYMPHAEUM = 170,
    BUILDING_SMALL_MAUSOLEUM = 171,
    BUILDING_LARGE_MAUSOLEUM = 172,
    BUILDING_WATCHTOWER = 173,
    BUILDING_PALISADE = 174,
    BUILDING_GARDEN_PATH = 175,
    BUILDING_CARAVANSERAI = 176,
    BUILDING_ROOFED_GARDEN_WALL = 177,
    BUILDING_ROOFED_GARDEN_WALL_GATE = 178,
    BUILDING_HEDGE_GATE_DARK = 179,
    BUILDING_HEDGE_GATE_LIGHT = 180,
    BUILDING_PALISADE_GATE = 181,
    BUILDING_GLADIATOR_STATUE = 182,
    BUILDING_HIGHWAY = 183,
    BUILDING_GOLD_MINE = 184,
    BUILDING_CITY_MINT = 185,
    BUILDING_DEPOT = 186,
    BUILDING_SAND_PIT = 187,
    BUILDING_STONE_QUARRY = 188,
    BUILDING_CONCRETE_MAKER = 189,
    BUILDING_BRICKWORKS = 190,
    BUILDING_PANELLED_GARDEN_WALL = 191,
    BUILDING_PANELLED_GARDEN_GATE = 192,
    BUILDING_LOOPED_GARDEN_GATE = 193,
    // helper constants
    BUILDING_TYPE_MAX = 194
} building_type;

/**
 * House levels
 */
typedef enum {
    HOUSE_MIN = 0,
    HOUSE_SMALL_TENT = 0,
    HOUSE_LARGE_TENT = 1,
    HOUSE_SMALL_SHACK = 2,
    HOUSE_LARGE_SHACK = 3,
    HOUSE_SMALL_HOVEL = 4,
    HOUSE_LARGE_HOVEL = 5,
    HOUSE_SMALL_CASA = 6,
    HOUSE_LARGE_CASA = 7,
    HOUSE_SMALL_INSULA = 8,
    HOUSE_MEDIUM_INSULA = 9,
    HOUSE_LARGE_INSULA = 10,
    HOUSE_GRAND_INSULA = 11,
    HOUSE_SMALL_VILLA = 12,
    HOUSE_MEDIUM_VILLA = 13,
    HOUSE_LARGE_VILLA = 14,
    HOUSE_GRAND_VILLA = 15,
    HOUSE_SMALL_PALACE = 16,
    HOUSE_MEDIUM_PALACE = 17,
    HOUSE_LARGE_PALACE = 18,
    HOUSE_LUXURY_PALACE = 19,
    HOUSE_MAX = 19,
} house_level;

enum {
    BUILDING_STATE_UNUSED = 0,
    BUILDING_STATE_IN_USE = 1,
    BUILDING_STATE_UNDO = 2,
    BUILDING_STATE_CREATED = 3,
    BUILDING_STATE_RUBBLE = 4,
    BUILDING_STATE_DELETED_BY_GAME = 5, // used for earthquakes, fires, house mergers
    BUILDING_STATE_DELETED_BY_PLAYER = 6,
    BUILDING_STATE_MOTHBALLED = 7
};

#endif // BUILDING_TYPE_H
