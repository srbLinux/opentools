#include "messagebox.h"

#include "../defines.h"
#include "../global.h"

#include "../kernel/font.h"
#include "../kernel/style.h"
#include "../kernel/debug.h"

void OTS_MessageBox_Warning(const char *content, PX_Object *parentWidget, OTS_Font *font) {
    px_texture icon, scaleIcon;
    px_color backgroundColor;
    int textWidth = 0, textHeight = 0;
    int pWidth = PX_Object_WidgetGetRenderTargetHeight(parentWidget);
    if (parentWidget==root) {
        pWidth = WINDOW_DEFAULT_WIDTH;
    }
    textWidth = OTS_FontTextWidth(font, content), textHeight = OTS_FontTextHeight(font, content);
    if (textWidth < 400) textWidth = 400;
    if (textHeight < 60) textHeight = 60;
    OTS_DEBUG("pWidth: %d, width: %d, height: %d.\n", pWidth, textWidth, textHeight);
    PX_Object *warning = PX_Object_WidgetCreate(mp, parentWidget, (pWidth - textWidth)/2, 10, textWidth, textHeight, "", font->fontModule);
    OTS_DEBUG("warning widget create, %d.\n", (warning!=NULL));
    int load = PX_FALSE;
    switch (gs->style) {
    case SOFTWARE_DARK:
        // backgroundColor = PX_COLOR(255, 72, 72, 78);
        load = PX_LoadTextureFromFile(mp, &icon, "./resources/icons/err_daily.png");
        break;
    case SOFTWARE_DAILY:
    default:
        // backgroundColor = PX_COLOR(255, 255, 255, 255);
        load = PX_LoadTextureFromFile(mp, &icon, "./resources/icons/err_daily.png");
        // PX_TextureRender();
        break;
    }
    OTS_DEBUG("load: %d.\n", load);
    PX_TextureCreateScale(mp, &icon, 25, 25, &scaleIcon);
    PX_Object *panel = PX_Object_PanelCreate(mp, warning, 0, 0, textWidth, textHeight);
    px_surface *pTarget = PX_Object_PanelGetSurface(panel);
    PX_GeoDrawRect(pTarget, 0, 0, textWidth, textHeight, gs->backgroundColor);
    PX_TextureRender(pTarget, &scaleIcon, 15, 15, PX_ALIGN_CENTER, NULL);
    PX_Object_WidgetShow(warning);
}


void OTS_MessageBox_Critical(const char *content, PX_Object *parentWidget, OTS_Font *font) {
    
}
