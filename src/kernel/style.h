#ifndef _OPENTOOLS_STYLE_H_
#define _OPENTOOLS_STYLE_H_

#include <PainterEngine.h>

#include "./font.h"

enum OTS_Software_Style {
    SOFTWARE_DAILY, SOFTWARE_DARK,
};

typedef struct __OTS_Style {
    px_color textColor;
    px_color backgroundColor;
    OTS_Font *textFont;
    enum OTS_Software_Style style;
} OTS_Style;

extern int OTS_Style_Software_Switch(OTS_Style *style);
extern OTS_Style *OTS_Style_Software_Create(enum OTS_Software_Style style);
extern OTS_Style *OTS_Style_Create(px_color textColor, px_color backgroundColor, OTS_Font *textFont);

#endif