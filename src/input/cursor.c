#include "cursor.h"

#include "building/construction.h"
#include "game/system.h"
#include "window/city.h"

#define CURSOR_PATH_NAME "Cursors/Color_Cursors"

static const cursor ARROW[CURSOR_SCALE_MAX][CURSOR_TYPE_MAX] = {
    {
        { CURSOR_TYPE_PNG, 25, 25, 108, 0, 0, 0, 0, CURSOR_PATH_NAME ".png" },
        { CURSOR_TYPE_PNG, 25, 25, 0, 0, 0, 0, 0, CURSOR_PATH_NAME "/Arrow.png" },
        {
            CURSOR_TYPE_PIXMAP, 13, 21, 0, 0, 0, 0, 0,
            "#            "
            "##           "
            "#'#          "
            "#''#         "
            "#'''#        "
            "#''''#       "
            "#'''''#      "
            "#''''''#     "
            "#'''''''#    "
            "#''''''''#   "
            "#'''''''''#  "
            "#''''''''''# "
            "#''''''######"
            "#'''#''#     "
            "#''# #''#    "
            "#'#  #''#    "
            "##    #''#   "
            "#     #''#   "
            "       #''#  "
            "       #''#  "
            "        ##   "
        },
    },
    {
        { CURSOR_TYPE_PNG, 34, 34, 90, 98, 0, 0, 0, CURSOR_PATH_NAME ".png" },
        { CURSOR_TYPE_PNG, 34, 34, 0, 0, 0, 0, 0, CURSOR_PATH_NAME "/Arrow_150.png" },
        {
            CURSOR_TYPE_PIXMAP, 18, 30, 0, 0, 0, 0, 0,
            "#                 "
            "##                "
            "#&#               "
            "#'&#              "
            "#''&#             "
            "#'''&#            "
            "#''''&#           "
            "#'''''&#          "
            "#''''''&#         "
            "#'''''''&#        "
            "#''''''''&#       "
            "#'''''''''&#      "
            "#''''''''''&#     "
            "#'''''''''''&#    "
            "#''''''''''''&#   "
            "#'''''''''''''&#  "
            "#''''''''''''''&# "
            "#'''''''''''''''$#"
            "#'''''''&#########"
            "#''''$%''$        "
            "#'''$##&'&#       "
            "#''$#  $''%#      "
            "#&#    #&''#      "
            "##      #''%#     "
            "#       #%''#     "
            "         #''&#    "
            "         #%''$    "
            "          #&'&#   "
            "           $'&#   "
            "           ###    "
        }
    },
    {
        { CURSOR_TYPE_PNG, 44, 44, 46, 60, 0, 0, 0, CURSOR_PATH_NAME ".png" },
        { CURSOR_TYPE_PNG, 44, 44, 0, 0, 0, 0, 0, CURSOR_PATH_NAME "/Arrow_200.png" },
        {
            CURSOR_TYPE_PIXMAP, 24, 40, 0, 0, 0, 0, 0,
            "#                       "
            "##                      "
            "#&#                     "
            "#'&#                    "
            "#''&#                   "
            "#'''&#                  "
            "#''''&#                 "
            "#'''''&#                "
            "#''''''&#               "
            "#'''''''&#              "
            "#''''''''&#             "
            "#'''''''''&#            "
            "#''''''''''&#           "
            "#'''''''''''&#          "
            "#''''''''''''&#         "
            "#'''''''''''''&#        "
            "#''''''''''''''&#       "
            "#'''''''''''''''&#      "
            "#''''''''''''''''&#     "
            "#'''''''''''''''''&#    "
            "#''''''''''''''''''&#   "
            "#'''''''''''''''''''&#  "
            "#''''''''''''''''''''&# "
            "#'''''''''''''''''''''&#"
            "#''''''''''&############"
            "#''''''&&'''$           "
            "#'''''&##'''&#          "
            "#''''&# #%'''%#         "
            "#''''#   #''''#         "
            "#'''#    #%'''%#        "
            "#''#      #&'''#        "
            "#'$#       $'''&#       "
            "#$#        #&'''$       "
            "##          #'''&#      "
            "#           #%'''%#     "
            "             #''''#     "
            "             #%'''%#    "
            "              #&'''#    "
            "               $'&$#    "
            "               ###      "
        }
    }
};

static const cursor SWORD[CURSOR_SCALE_MAX][CURSOR_TYPE_MAX] = {
    {
        { CURSOR_TYPE_PNG, 26, 26,0, 106, 0, 0, 0, CURSOR_PATH_NAME ".png" },
        { CURSOR_TYPE_PNG, 26, 26, 0, 0, 0, 0, 0, CURSOR_PATH_NAME "/Sword.png" },
        {
            CURSOR_TYPE_PIXMAP, 22, 22, 0, 0, 0, 0, 0,
            "#####                 "
            "#'''##                "
            "#''''##               "
            "#'''''##              "
            "##'''''##             "
            " ##'''''##            "
            "  ##'''''##           "
            "   ##'''''##          "
            "    ##'''''##         "
            "     ##'''''##        "
            "      ##'''''##       "
            "       ##'''''##      "
            "        ##'''''#####  "
            "         ##'''''#''#  "
            "          ##'''#'''#  "
            "           ##'#'''##  "
            "            ##'''###  "
            "            #'''##'###"
            "            #''##''''#"
            "            ######'''#"
            "                 #''##"
            "                 #### "
        }
    },
    {
        { CURSOR_TYPE_PNG, 38, 38, 90, 60, 0, 0, 0, CURSOR_PATH_NAME ".png" },
        { CURSOR_TYPE_PNG, 38, 38, 0, 0, 0, 0, 0, CURSOR_PATH_NAME "/Sword_150.png" },
        {
            CURSOR_TYPE_PIXMAP, 34, 34, 0, 0, 0, 0, 0,
            "######                            "
            "#%&&&##                           "
            "#&'''&##                          "
            "#&''''&##                         "
            "#&'''''&##                        "
            "##&'''''&##                       "
            " ##&'''''&##                      "
            "  ##&'''''&##                     "
            "   ##&'''''&##                    "
            "    ##&'''''&##                   "
            "     ##&'''''&##                  "
            "      ##&'''''&##                 "
            "       ##&'''''&##                "
            "        ##&'''''&##               "
            "         ##&'''''&##              "
            "          ##&'''''&##             "
            "           ##&'''''&##            "
            "            ##&'''''&##           "
            "             ##&'''''&##          "
            "              ##&'''''&########   "
            "               ##&''''''&##&&$#   "
            "                ##&''''&##&''$#   "
            "                 ##&''&##&''&##   "
            "                  ##%&##&''&###   "
            "                   #&##&''&####   "
            "                   ###&''&#####   "
            "                   ##&''&###%$### "
            "                   #&''&###%'%$$##"
            "                   #&'&###%'''''$#"
            "                   #$$####$%''''$#"
            "                   ########$'''&##"
            "                          #$''&#  "
            "                          ##$$##  "
            "                           ####   "
        }
    },
    {
        { CURSOR_TYPE_PNG, 48, 48, 60, 0, 0, 0, 0, CURSOR_PATH_NAME ".png" },
        { CURSOR_TYPE_PNG, 48, 48, 0, 0, 0, 0, 0, CURSOR_PATH_NAME "/Sword_200.png" },
        {
            CURSOR_TYPE_PIXMAP, 46, 44, 0, 0, 0, 0, 0,
            "########                                      "
            "#%%%%%%##                                     "
            "#%'''''&##                                    "
            "#%''''''&##                                   "
            "#%'''''''%##                                  "
            "#%''''''''%##                                 "
            "#%'''''''''%##                                "
            "##%'''''''''%##                               "
            " ##%'''''''''%##                              "
            "  ##%'''''''''%##                             "
            "   ##%'''''''''%##                            "
            "    ##%'''''''''%##                           "
            "     ##%'''''''''%##                          "
            "      ##%'''''''''%##                         "
            "       ##%'''''''''%##                        "
            "        ##%'''''''''%##                       "
            "         ##%'''''''''%##                      "
            "          ##%'''''''''%##                     "
            "           ##%'''''''''%##                    "
            "            ##%'''''''''%##                   "
            "             ##%'''''''''%##                  "
            "              ##%'''''''''%##                 "
            "               ##%'''''''''%##                "
            "                ##%'''''''''%##               "
            "                 ##%'''''''''%##              "
            "                  ##%'''''''''%##########     "
            "                   ##%'''''''''%$$##$$$##     "
            "                    ##%''''''''''%#%'''$#     "
            "                     ##%''''''''%#%''''$#     "
            "                      ##%''''''%#%'''''$#     "
            "                       ##%''''%#%'''''%##     "
            "                        ##%''%#%'''''%###     "
            "                         #$'%#%'''''%####     "
            "                         #$%#%'''''%#####     "
            "                         ###%'''''%###%$#     "
            "                         ##%'''''%###%'$####  "
            "                         #$'''''%###%''%$$$## "
            "                         #$''''%###%'''''''$##"
            "                         #$'''%###%''''''''$##"
            "                         ##$$$####$$%'''''''$#"
            "                         ###########$''''$####"
            "                                   #$'''$&#   "
            "                                    #$''$#    "
            "                                     #####    "
        }
    }
};

static const cursor SHOVEL[CURSOR_SCALE_MAX][CURSOR_TYPE_MAX] = {
    {
        { CURSOR_TYPE_PNG, 32, 32, 46, 104, 1, 26, 0, CURSOR_PATH_NAME ".png" },
        { CURSOR_TYPE_PNG, 32, 32, 0, 0, 1, 26, 0, CURSOR_PATH_NAME "/Shovel.png" },
        {
            CURSOR_TYPE_PIXMAP, 28, 28, 0, 0, 1, 26, 0,
            "                       ##   "
            "                      ####  "
            "                     ##'### "
            "                     #'''###"
            "                     ##'''##"
            "                    ####'## "
            "                   ##'####  "
            "                  ##'##     "
            "                 ##'##      "
            "                ##'##       "
            "               ##'##        "
            "              ##'##         "
            "             ##'##          "
            "       #    ##'##           "
            "      #### ##'##            "
            "     #''####'##             "
            "    #''''##'##              "
            "   #''''##'##               "
            "  #''''#'#####              "
            " #''''#'''#'##              "
            " #'''#'''#'''##             "
            "#'''''''#''''#              "
            "#''''''#''''#               "
            "#''''''''''#                "
            "#'''''''''#                 "
            "#''''''''#                  "
            " #'''''##                   "
            "  #####                     "
        }
    },
    {
        { CURSOR_TYPE_PNG, 46, 46, 0, 60, 2, 39, 0, CURSOR_PATH_NAME ".png" },
        { CURSOR_TYPE_PNG, 46, 46, 0, 0, 2, 39, 0, CURSOR_PATH_NAME "/Shovel_150.png" },
        {
            CURSOR_TYPE_PIXMAP, 44, 41, 0, 0, 2, 39, 0,
            "                                   ####     "
            "                                 #######    "
            "                                ##%&#####   "
            "                               ##&''$#####  "
            "                               #$'''''$#### "
            "                               #$''''''$####"
            "                               ##&''''''%## "
            "                              ####&''''$##  "
            "                             ##$''##&&&$#   "
            "                            ##$'''$#####    "
            "                           ##$'''$##        "
            "                          ##$'''$##         "
            "                         ##$'''$##          "
            "                        ##$'''$##           "
            "                       ##$'''$##            "
            "                      ##$'''$##             "
            "                     ##$'''$##              "
            "                    ##$'''$##               "
            "         ###       ##$'''$##                "
            "        ##$##     ##$'''$##                 "
            "       ##%'%##   ##$'''$##                  "
            "      ##%'''%## ##$'''$##                   "
            "     ##%'''''%##%''''$##                    "
            "    ##%''''''%#$''''$##                     "
            "   ##%''''''%#$''''$##                      "
            "  ##%''''''%#$#%''$##                       "
            "  #$''''''%#%'%#$###                        "
            "  #&'''''%#%'''$#%%##                       "
            " ##'''''%#%'''%#%''%##                      "
            " #$''''%#%'''%#%''''%##                     "
            " #&''''%%'''%#%''''''$#                     "
            " #&''''''''%#%''''''%##                     "
            "##''''''''%#%''''''%##                      "
            "#$''''''''%%''''''%##                       "
            "#$'''''''''''''''%##                        "
            "#$''''''''''''''%##                         "
            "#$'''''''''''''%##                          "
            "##'''''''''''&$##                           "
            " #%''''''''&$###                            "
            " ##%&''&%$###                               "
            "  #########                                 "
        }
    },
    {
        { CURSOR_TYPE_PNG, 60, 60, 0, 0, 3, 52, 0, CURSOR_PATH_NAME ".png" },
        { CURSOR_TYPE_PNG, 60, 60, 0, 0, 3, 52, 0, CURSOR_PATH_NAME "/Shovel_200.png" },
        {
            CURSOR_TYPE_PIXMAP, 58, 55, 0, 0, 3, 52, 0,
            "                                               ##         "
            "                                             ######       "
            "                                            ###&#####     "
            "                                           ##$''&#####    "
            "                                          ##$''''&#####   "
            "                                         ##$'''''''$####  "
            "                                         ##$''''''''$#### "
            "                                         ##$'''''''''$####"
            "                                         ##$''''''''''$###"
            "                                         ###%'''''''''$## "
            "                                        ##%%#%''''''&##   "
            "                                       ##&''%#%''''&##    "
            "                                      ##&''''%#$$$$##     "
            "                                     ##&''''%#######      "
            "                                    ##&''''&###           "
            "                                   ##&''''&##             "
            "                                  ##&''''&##              "
            "                                 ##&''''&##               "
            "                                ##&''''&##                "
            "                               ##&''''&##                 "
            "                              ##&''''&##                  "
            "                             ##&''''&##                   "
            "                            ##&''''&##                    "
            "                           ##&''''&##                     "
            "             ##           ##&''''&##                      "
            "            ####         ##&''''&##                       "
            "           ##%%##       ##&''''&##                        "
            "          ##%''%##     ##&''''&##                         "
            "         ##%''''%##   ##&''''&##                          "
            "        ##%''''''%## ##%''''&##                           "
            "       ##%''''''''%##%'''''&##                            "
            "      ##%'''''''''%#$'''''&##                             "
            "     ##%'''''''''%#%'''''&##                              "
            "    ##%'''''''''%##%''''%##                               "
            "   ##%'''''''''%#%%#%''$##                                "
            "   #$'''''''''%#%''%#%%###                                "
            "   #&''''''''%#%''''%##%%##                               "
            "  ##''''''''%#%'''''%#%''%##                              "
            "  #%'''''''%#%'''''%#%''''%##                             "
            "  #&''''''%#%'''''%#%''''''%##                            "
            "  #''''''&#%'''''%#%''''''''%##                           "
            " #$'''''''&'''''%#%'''''''''%##                           "
            " #%''''''''''''%#%'''''''''%##                            "
            " #&'''''''''''%#%'''''''''%##                             "
            " #&''''''''''&#%'''''''''%##                              "
            " #''''''''''''&'''''''''%##                               "
            " #'''''''''''''''''''''%##                                "
            "##''''''''''''''''''''%##                                 "
            " #'''''''''''''''''''%##                                  "
            " #&'''''''''''''''''%##                                   "
            " #&'''''''''''''''&$##                                    "
            " #$'''''''''''''&$###                                     "
            "  #%'''''''''&%###                                        "
            "   #$%&&&&%$####                                          "
            "    #########                                             "
        }
    }
};

const cursor *input_cursor_data(cursor_shape cursor_id, cursor_scale scale)
{
    switch (cursor_id) {
        case CURSOR_ARROW: return ARROW[scale];
        case CURSOR_SHOVEL: return SHOVEL[scale];
        case CURSOR_SWORD: return SWORD[scale];
        default: return 0;
    }
}

void input_cursor_update(window_id window)
{
    if (window == WINDOW_CITY_MILITARY && !window_city_military_is_cursor_in_menu()) {
        system_set_cursor(CURSOR_SWORD);
    } else if (window == WINDOW_CITY && building_construction_type() == BUILDING_CLEAR_LAND) {
        system_set_cursor(CURSOR_SHOVEL);
    } else {
        system_set_cursor(CURSOR_ARROW);
    }
}
