#ifndef _OPENTOOLS_ABOUTSOFTWAREWIDGET_H
#define _OPENTOOLS_ABOUTSOFTWAREWIDGET_H

#include <PainterEngine.h>

typedef struct __OTS_AboutSoftwareWidget {
    PX_Object *widget;
} OTS_AboutSoftwareWidget;

extern OTS_AboutSoftwareWidget *OTS_AboutSoftwareWidget_Initialize(int width, int height);

#endif