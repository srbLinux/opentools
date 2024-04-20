#ifndef _OPENTOOLS_IMAGETOPDFWIDGET_H_
#define _OPENTOOLS_IMAGETOPDFWIDGET_H_

#include <PainterEngine.h>

typedef struct __OTS_ImageToPDFWidget {
    PX_Object *widget;
} OTS_ImageToPDFWidget;

extern OTS_ImageToPDFWidget *OTS_ImageToPDFWidget_Initialize(int width, int height);

#endif