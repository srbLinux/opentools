#include "messagebox.h"
#include "../defines.h"
#include "../kernel/style.h"

void OTS_MessageBox_Warning(const char *content, PX_Object *parentWidget, OTS_Font *font, int style) {
    px_texture iconTexture;
    px_color backgroundColor;
    int textWidth = 0, textHeight = 0;
    int pWidth = PX_Object_WidgetGetRenderTargetHeight(parentWidget);
    PX_Object *warning = PX_Object_WidgetCreate(mp, parentWidget, (pWidth - textWidth)/2, 10, textWidth, textHeight, "", NULL);

    switch (style) {
    case SOFTWARE_DARK:
        backgroundColor = PX_COLOR(255, 72, 72, 78);
        PX_LoadTextureFromFile(mp, &iconTexture, "../../resources/icons/");
        break;
    case SOFTWARE_DAILY:
    default:
        backgroundColor = PX_COLOR(255, 255, 255, 255);
        PX_LoadTextureFromFile(mp, &iconTexture, "../../resources/icons/err_daily.png");
        // PX_TextureRender();
        break;
    }

}


void OTS_MessageBox_Critical(const char *content, PX_Object *parentWidget, OTS_Font *font, int style) {
    
}
