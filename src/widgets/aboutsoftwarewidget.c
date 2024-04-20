#include "./aboutsoftwarewidget.h"
#include "../defines.h"
#include "../global.h"

OTS_AboutSoftwareWidget *OTS_AboutSoftwareWidget_Initialize(int width, int height) {
    OTS_AboutSoftwareWidget *aboutWidget = (OTS_AboutSoftwareWidget *)MP_Malloc(mp, sizeof(OTS_AboutSoftwareWidget));
    OTS_printf("%s: OTS_AboutSoftwareWidget object initialize. check %d\n", __func__, (aboutWidget!=NULL));
    aboutWidget->widget = OTS_DEFUALT_WIDGET_CREATE;
    OTS_printf("%s: OTS_AboutSoftwareWidget::widget object initialize, check %d\n", __func__, (aboutWidget->widget!=NULL));
    const char widgetTitle[] = "About Software";
    OTS_Font *titleFont = OTS_FontStdCreate(FONT_FangSong, 20);
    int titleWidth = OTS_FontTextWidth(titleFont, widgetTitle);
    int titleHeight = OTS_FontTextHeight(titleFont, widgetTitle);
    PX_Object *label = PX_Object_LabelCreate(mp, aboutWidget->widget, 20, 20, titleWidth, titleHeight,
                                                                   widgetTitle, titleFont->fontModule, gs->textColor);
    const char labelText1[] = "  This software is a collection of small tools written in C language."
        "It includes utilities for manipulating images, files, and other related tasks."
        "For example, it can convert images to PDF files.";
    label = PX_Object_AutoTextCreate(mp, aboutWidget->widget, 10, titleHeight+40, width-20, gs->textFont->fontModule);
    PX_Object_AutoTextSetTextColor(label, gs->textColor); PX_Object_AutoTextSetText(label, labelText1);
    return aboutWidget;
}