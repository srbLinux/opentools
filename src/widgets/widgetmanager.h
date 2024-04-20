#ifndef _OPENTOOLS_WIDGETMANAGER_H
#define _OPENTOOLS_WIDGETMANAGER_H

#include <PainterEngine.h>

#include "../kernel/vector.h"

typedef struct __OTS_WidgetManager {
    int nowWidget;
    int width, height;
    OTS_Vector *widgets;
    PX_Object *selfWidget;
} OTS_WidgetManager;

extern OTS_WidgetManager *OTS_WidgetManager_Initialize(int width, int height);
extern int OTS_WidgetManager_AddWidget(OTS_WidgetManager *manager, PX_Object *widget);
extern px_bool OTS_WidgetManager_WidgetShowByIndex(OTS_WidgetManager *manager, int index);
extern px_bool OTS_WidgetManager_WidgetShowByObject(OTS_WidgetManager *manager, PX_Object *widget);

#endif