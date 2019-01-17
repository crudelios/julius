#include "cursor.h"

#include "building/construction.h"
#include "game/system.h"

static const cursor ARROW[] = {
    {
        0, 0, 13, 21,
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
    {
        0, 0, 18, 30,
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
    },
    {
        0, 0, 24, 40,
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
};

static const cursor SWORD[] = {
    {
        0, 0, 22, 22,
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
    },
    {
        0, 0, 34, 34,
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
    },
    {
        0, 0, 46, 44,
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
};

static const cursor SHOVEL[] = {
    {
        1, 26, 28, 28,
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
    },
    {
        2, 39, 44, 41,
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
    },
    {
        3, 52, 58, 55,
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
};

static cursor_scale input_cursor_get_scale(double scale)
{
    if (scale <= 1.25f)
        return CURSOR_SCALE_1;

    if (scale <= 1.75f)
        return CURSOR_SCALE_1_5;

    return CURSOR_SCALE_2;
}

const cursor *input_cursor_data(cursor_shape cursor_id, double scale)
{
    switch (cursor_id) {
        case CURSOR_ARROW: return &ARROW[input_cursor_get_scale(scale)];
        case CURSOR_SHOVEL: return &SHOVEL[input_cursor_get_scale(scale)];
        case CURSOR_SWORD: return &SWORD[input_cursor_get_scale(scale)];
        default: return 0;
    }
}

void input_cursor_update(window_id window)
{
    if (window == WINDOW_CITY_MILITARY) {
        system_set_cursor(CURSOR_SWORD);
    } else if (window == WINDOW_CITY && building_construction_type() == BUILDING_CLEAR_LAND) {
        system_set_cursor(CURSOR_SHOVEL);
    } else {
        system_set_cursor(CURSOR_ARROW);
    }
}
