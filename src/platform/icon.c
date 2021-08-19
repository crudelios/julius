#include "icon.h"

#include <stddef.h>

static uint8_t icon_pixels[] = 
    "\377\377\377\000\377\377\377\000\373\372\370\000\325\304\236\011\260\243\212U\232"
    "\220u\256\224\201c\300\221\203d\365\233\220z\377\217\177d\343\224\212q\251"
    "\322\314\276Z\315\305\226\011\374\373\371\000\377\377\377\000\377\377\377\000\377"
    "\377\377\000\376\375\371\000\301\267\237#\212|W\327\204rW\377\224\211x\377\247"
    "\231\203\377\274\262\235\377\277\266\242\377\254\241\211\377\202rI\377wa"
    "\070\377\237\224~\346\311\277\244N\377\376\372\000\377\377\377\000\376\375\373"
    "\000\267\257\236\034\234\213_\346eR\037\377\220\202c\377\303\274\252\377\306"
    "\273\242\377\364\361\350\377\264\253\227\377\257\240{\377\236\226\177\377"
    "mX\"\377\226y?\377\200u]\377\243\225pY\374\373\367\000\371\370\363\000\304\271"
    "\235\247\214n\062\377rc@\377\226\213n\377\330\315\262\377\334\322\264\377"
    "\307\300\256\377\260\245\212\377\254\242\203\377VO\066\377{e/\377\234\202"
    "P\377rhW\377\213\177g\365\262\243{'\233\220j\031\273\252\212\376z`$\377na"
    "?\377\247\233z\377\305\271\226\377\317\306\251\377\310\302\251\377\256\240"
    "}\377\232\213]\377WM.\377t\\&\377\221}V\377\235\215i\377|oM\377\234\221o"
    "\246\276\262\237\204\274\247|\377\223s\063\377gZ:\377\261\243}\377\253\240"
    "\202\377d\\:\377\244\232}\377\224\204W\377\205{Z\377G?%\377\224zA\377\274"
    "\241e\377\222\204h\377\205|i\377\222\201X\366\247\231u\326\215\200]\377\246"
    "\214Q\377c_M\377\217\206n\377\250\237\203\377m_\066\377\240\224n\377\245\223"
    "a\377j_A\377\231\206^\377\276\255\200\377\227\177B\377\207uS\377\231\177"
    "G\377\226|R\377\243\224u\375\275\251\200\377\255\226W\377ue=\377i\\>\377"
    "\204z[\377\244\231{\377\177pC\377\304\262\200\377\225\204X\377\271\254\206"
    "\377\261\241w\377\210x^\377\236\215h\377\217\211~\377uc@\365\220\177]\377"
    "\233\216t\377\254\226Z\377\225~C\377wjH\377\264\253\216\377\211|^\377\224"
    "\201K\377\277\254u\377\212xA\377\243\221^\377mbL\377|g/\377\236\215k\377"
    "\227\200M\377\227\204\\\317\236\213_\376\244\232\204\377\246\222Z\377\241"
    "\213R\377_O%\377|pK\377\231\211[\377\203qB\377\253\230d\377\221{C\377\204"
    "sC\377mcN\377\241\213N\377\254\235|\377\202oS\377\213\200f\255\234\213h\301"
    "\332\316\262\377\256\232k\377\247\225j\377\217x@\377XE\031\377\251\230j\377"
    "\316\274\211\377tc\065\377/%\013\377\204vR\377xj@\377\236\211N\377\273\255"
    "\206\377\253\236\203\377\204tP\217~u\\z\237\213c\377\307\261\202\377\266"
    "\246\205\377\237\210]\377\204i\063\377\246\226h\377\275\254z\377\250\225`"
    "\377D\070\030\377}g:\377\231\206T\377\260\233k\377\226\204^\377td@\377\224"
    "\215vM\255\247\213\020\241\226~\353\240\217o\377\246\226x\377\226\206c\377"
    "\240\210X\377\332\314\250\377\264\244|\377\231\207S\377kW*\377\264\240i\377"
    "\263\241m\377\301\260\215\377\252\226y\377\223\211r\322\377\377\352\000\376"
    "\376\374\000\211\206oI\211qG\367\226\205_\377\212x\\\377\267\254\220\377\260"
    "\232j\377\355\341\302\377vf;\377uY'\377\313\273\217\377\307\271\227\377\250"
    "\227s\377l[\066\377\223\217|?\377\377\375\000\377\377\377\000\377\377\373\000\223"
    "\213jGg[=\365~f\061\377K\067\014\377\231\203T\377\353\332\255\377\306\273\230"
    "\377tU\"\377\276\257\225\377\237\213_\377i\\<\365\217\204`\\\377\377\373"
    "\000\377\376\373\000\377\377\377\000\377\377\377\000\371\370\366\000\237\227~\033tk"
    "J\207\203rP\336\207yS\377\253\232q\377\215~U\377ob:\365fW-\307wmC\207\204"
    "wI\025\373\372\366\000\377\377\377\000\377\377\377\000";

void *platform_icon_get_pixels(void)
{
    return icon_pixels;
}