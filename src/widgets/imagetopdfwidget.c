#include "imagetopdfwidget.h"

#include "../defines.h"
#include "../global.h"

#include <hpdf.h>

OTS_ImageToPDFWidget *OTS_ImageToPDFWidget_Initialize(int width, int height) {
    OTS_ImageToPDFWidget *pdfWidget = (OTS_ImageToPDFWidget *)MP_Malloc(mp, sizeof(OTS_ImageToPDFWidget));
    pdfWidget->widget = OTS_DEFUALT_WIDGET_CREATE;
    PX_Object *inFolderInput = PX_Object_EditCreate(mp, pdfWidget->widget, 20, 20, (width-40), 40, gs->textFont->fontModule);
    return pdfWidget;
}