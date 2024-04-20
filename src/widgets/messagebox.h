#ifndef _OPENTOOLS_MESSAGEBOX_H
#define _OPENTOOLS_MESSAGEBOX_H

#include <PainterEngine.h>

#include "../kernel/font.h"

extern void OTS_MessageBox_Warning(const char *content, PX_Object *parentWidget, OTS_Font *font, int style);
extern void OTS_MessageBox_Critical(const char *content, PX_Object *parentWidget, OTS_Font *font, int style);

#endif