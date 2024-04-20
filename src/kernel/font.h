#ifndef _OPENTOOLS_FONT_H
#define _OPENTOOLS_FONT_H

#include <PainterEngine.h>

typedef struct __OTS_Font {
    int size;
    char *family;
    PX_FontModule *fontModule;
} OTS_Font;

enum OTS_STD_Font {
    FONT_SimSunb, FONT_FangSong
};

extern int OTS_FontTextWidth(OTS_Font *font, const char *text);
extern int OTS_FontTextHeight(OTS_Font *font, const char *text);
extern OTS_Font *OTS_FontStdCreate(enum OTS_STD_Font font, int font_size);
extern OTS_Font *OTS_FontOtherCreate(const char *font_ttf_path, int font_size);


#endif