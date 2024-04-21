#ifndef _OPENTOOLS_SELECTFILESWIDGET_H_
#define _OPENTOOLS_SELECTFILESWIDGET_H_

#include <PainterEngine.h>

#include "../kernel/vector.h"

struct OTS_SelectFilesWidget_Private;

typedef struct __OTS_SelectFilesWidget {
    char *filter;
    OTS_Vector *texts;
    PX_Object *widget;
    int width, height;
    // Disallow other structs and functions to manipulate these data.
    struct OTS_SelectFilesWidget_Private *data;
} OTS_SelectFilesWidget;

extern OTS_SelectFilesWidget *OTS_SelectFilesWidget_Initialize(int width, int height, const char *name, const char *filter);
extern int OTS_SelectFilesWidget_SetFilter(OTS_SelectFilesWidget *widget, const char *filter);
extern const char *OTS_SelectFilesWidget_GetFilter(OTS_SelectFilesWidget *widget);
extern OTS_Vector *OTS_SelectFilesWidget_GetFilesPath(OTS_SelectFilesWidget *widget);
extern void OTS_SelectFilesWidget_SetFilesPath(OTS_SelectFilesWidget *widget, const char **filespath, int fileslen);
extern int OTS_SelectFilesWidget_AddFile(OTS_SelectFilesWidget *widget, const char *filepath);
extern int OTS_SelectFilesWidget_Hide(OTS_SelectFilesWidget *widget);
extern void OTS_SelectFilesWidget_Free(OTS_SelectFilesWidget *widget);

#endif