#include "style.h"
#include "../global.h"
#include "../defines.h"

extern OTS_Style *OTS_Style_Software_Create(enum OTS_Software_Style style) {
    OTS_Style *gStyle = (OTS_Style *)MP_Malloc(mp, sizeof(OTS_Style));
    OTS_debug(logFILE, "%s: OTS_Style object initialize. check %d\n", __func__, (gStyle!=NULL));
    switch (style) {
    case SOFTWARE_DAILY:
        gStyle->textColor = PX_COLOR(255, 0, 0, 0);
        gStyle->backgroundColor = PX_COLOR(255, 255, 255, 255);
        break;
    case SOFTWARE_DARK:
        gStyle->textColor = PX_COLOR(255, 255, 255, 255);
        gStyle->backgroundColor = PX_COLOR(255, 0, 0, 0);
    }
    gStyle->textFont = OTS_FontStdCreate(FONT_SimSunb, 18); gStyle->style = style;
    OTS_debug(logFILE, "%s: OTS_Style::textFont object initialize. check %d\n", __func__, (gStyle->textFont!=NULL));
    return gStyle;
}