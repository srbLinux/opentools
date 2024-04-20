#include "font.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../defines.h"
#include "./platform.h"

OTS_Font *OTS_FontStdCreate(enum OTS_STD_Font font_, int fontSize) {
    OTS_Font *font = MP_Malloc(mp, sizeof(OTS_Font));
    font->family = MP_Malloc(mp, sizeof(char) * 128);
    char *ttfPath = (char *)malloc(sizeof(char) * 512);
    OTS_printf("%s: buffer and OTS_Font object initialize. check %d.\n", __func__, (font!=NULL));
    switch (font_) {
    case FONT_SimSunb:
        strcpy(font->family, "SimSunb");
        strcpy(ttfPath, "./resources/ttfs/simsunb.ttf"); 
        break;
    case FONT_FangSong:
        strcpy(font->family, "FangSong");
        strcpy(ttfPath, "./resources/ttfs/fangsong.ttf");
        break;
    }
    FILE *fptr = fopen(ttfPath, "r"); int ttfLen = 0;
    if (fptr==NULL) {
        OTS_err_info("%s: cannot open file: %s, because program now path is %s.\n", __func__, ttfPath, OTS_System_GetCurrentPath());
        return NULL;
    }
    font->fontModule = MP_Malloc(mp, sizeof(PX_FontModule));
    // 初始化字体
    PX_FontModuleInitialize(mp, font->fontModule);
    PX_IO_Data io = PX_LoadFileToIOData(ttfPath);
    px_bool ttf = PX_FontModuleInitializeTTF(mp, font->fontModule, PX_FONTMODULE_CODEPAGE_UTF8, fontSize, io.buffer, io.size);
    OTS_printf("%s: PX_FontModuleLoad initialize, PX_FontModule::codePage index is %d, check %d.\n", __func__, font->fontModule->codePage, ttf);
    fclose(fptr);
    return font;
}

int OTS_FontTextWidth(OTS_Font *font, const char *text) {
    // PX_FontTextGetRenderWidthHeight(font->fontModule, text);
    int width, height;
    PX_FontModuleTextGetRenderWidthHeight(font->fontModule, text, &width, &height);
    return width;
}

int OTS_FontTextHeight(OTS_Font *font, const char *text) {
    int width, height;
    PX_FontModuleTextGetRenderWidthHeight(font->fontModule, text, &width, &height);
    return height;
}