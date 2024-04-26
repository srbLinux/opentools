#ifndef _OPENTOOLS_SELECTFILESWIDGET_H_
#define _OPENTOOLS_SELECTFILESWIDGET_H_

#include <PainterEngine.h>

#include "../kernel/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OTS_SelectFilesWidget_Private;

typedef struct __OTS_SelectFilesWidget {
    char *filter;
    OTS_Vector *texts;
    PX_Object *widget;
    int width, height;
    // Disallow other structs and functions to manipulate these data.
    struct OTS_SelectFilesWidget_Private *data;
} OTS_SelectFilesWidget;

typedef void (*OTS_SelectFilesWidget_Event)(void *uptr, void *data);

extern OTS_SelectFilesWidget *OTS_SelectFilesWidget_Initialize(int width, int height, const char *name, const char *filter);
extern void OTS_SelectFilesWidget_SetFilter(OTS_SelectFilesWidget *widget, const char *filter);
extern const char *OTS_SelectFilesWidget_GetFilter(OTS_SelectFilesWidget *widget);
extern OTS_Vector *OTS_SelectFilesWidget_GetFilesPath(OTS_SelectFilesWidget *widget);
extern void OTS_SelectFilesWidget_SetFilesPath(OTS_SelectFilesWidget *widget, const char **filespath, int fileslen);
extern void OTS_SelectFilesWidget_AddFile(OTS_SelectFilesWidget *widget, const char *filepath);

/**
 * @brief   The second parameter data of OTS_SelectFilesWidget_Event is the path of the newly added file.
 *          eg: void event(void *uptr, void *data) {
 *                  char *newfilepath = (char *)data;
 *                  // something to do
 *              }
*/
extern void OTS_SelectFilesWidget_ResgiterItemAddEvent(OTS_SelectFilesWidget *widget, OTS_SelectFilesWidget_Event event, void *uptr);

/**
 * @brief   The 
*/
extern void OTS_SelectFilesWidget_RegisterItemDeleteEvent(OTS_SelectFilesWidget *widget, OTS_SelectFilesWidget_Event event, void *uptr);

/**
 * 
*/
extern void OTS_SelectFilesWidget_RegisterItemChangeEvent(OTS_SelectFilesWidget *widget, OTS_SelectFilesWidget_Event event, void *uptr);
extern int OTS_SelectFilesWidget_Hide(OTS_SelectFilesWidget *widget);
extern void OTS_SelectFilesWidget_Free(OTS_SelectFilesWidget *widget);

#ifdef __cplusplus
}
#endif

#endif