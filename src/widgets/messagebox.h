#ifndef _OPENTOOLS_MESSAGEBOX_H
#define _OPENTOOLS_MESSAGEBOX_H

#include <PainterEngine.h>

#include "../kernel/font.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*MessageBox_Callback)(void *ptr);

extern void OTS_MessageBox_Warning(const char *content, PX_Object *parentWidget, OTS_Font *font);
extern void OTS_MessageBox_Critical(const char *content, PX_Object *parentWidget, OTS_Font *font);
extern void OTS_MessageBox_Information(const char *content, PX_Object *parentWidget, OTS_Font *font);

#ifdef __cplusplus
}
#endif

#endif