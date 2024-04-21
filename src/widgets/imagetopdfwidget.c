#include "imagetopdfwidget.h"

#include "../defines.h"
#include "../global.h"

#include "../kernel/font.h"

#include "selectfileswidget.h"

#include <hpdf.h>

static void selectFilesCallback(PX_Object *obj, PX_Object_Event event, void *data);

OTS_ImageToPDFWidget *OTS_ImageToPDFWidget_Initialize(int width, int height) {
    OTS_ImageToPDFWidget *pdfWidget = (OTS_ImageToPDFWidget *)MP_Malloc(mp, sizeof(OTS_ImageToPDFWidget));
    pdfWidget->widget = OTS_DEFUALT_WIDGET_CREATE;
    const char *filter = "*.png;*.jpg;*.svg;*.jpeg;*.jpe;";
    OTS_SelectFilesWidget *selectImagesWidget = OTS_SelectFilesWidget_Initialize(width, height, "Select image files", filter);
    PX_Object_WidgetHide(selectImagesWidget->widget);
    char *labelText = (char *)malloc(sizeof(char) * 1024);
    int labelWidth = 0, labelHeight = 0, inputWidth = 0, inputHeight = 0;
    strcpy(labelText, "Input Images: ");
    labelWidth = OTS_FontTextWidth(gs->textFont, labelText);
    labelHeight = OTS_FontTextHeight(gs->textFont, labelText);
    PX_Object *label = PX_Object_LabelCreate(mp, pdfWidget->widget, 20, 20, labelWidth, labelHeight+8, 
                                                        labelText, gs->textFont->fontModule, gs->textColor);
    inputWidth = (width - 290), inputHeight = 25;
    PX_Object *input = PX_Object_EditCreate(mp, pdfWidget->widget, 30+labelWidth, 20, inputWidth, inputHeight, gs->textFont->fontModule);
    PX_Object *button = PX_Object_PushButtonCreate(mp, pdfWidget->widget, inputWidth+45+labelWidth, 20, 120, inputHeight, "Find Images", gs->textFont->fontModule);
    PX_ObjectRegisterEvent(button, PX_OBJECT_EVENT_EXECUTE, selectFilesCallback, selectImagesWidget);
    return pdfWidget;
}

void selectFilesCallback(PX_Object *obj, PX_Object_Event event, void *data) {
    OTS_SelectFilesWidget *widget = (OTS_SelectFilesWidget *)data;
    PX_Object_WidgetShow(widget->widget);
}