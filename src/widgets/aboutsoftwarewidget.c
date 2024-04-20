#include "./aboutsoftwarewidget.h"
#include "../defines.h"
#include "../global.h"

OTS_AboutSoftwareWidget *OTS_AboutSoftwareWidget_Initialize(int width, int height) {
    OTS_AboutSoftwareWidget *aboutWidget = (OTS_AboutSoftwareWidget *)MP_Malloc(mp, sizeof(OTS_AboutSoftwareWidget));
    OTS_printf("%s: OTS_AboutSoftwareWidget object initialize. check %d\n", __func__, (aboutWidget!=NULL));
    aboutWidget->widget = OTS_DEFUALT_WIDGET_CREATE;
    OTS_printf("%s: OTS_AboutSoftwareWidget::widget object initialize, check %d\n", __func__, (aboutWidget->widget!=NULL));
    const char labelText[] = "这是集合了一些小工具的软件，使用C语言写成。\n其中有图片、文件等相关的小工具，比如可以将图片转化成PDF。\n";
    int labelWidth = 400, labelHeight = 80;
    // PX_JsonGetString();
    PX_Object *label = PX_Object_LabelCreate(mp, aboutWidget->widget, (width-labelWidth)/2, 20, labelWidth, labelHeight, "This", NULL, gs->textColor);
    PX_Object *panel = PX_Object_PanelCreate(mp, label, 0, 0, labelWidth, labelHeight);
    PX_FontModuleDrawText(PX_Object_PanelGetSurface(panel), gs->textFont->fontModule, 10, 10, PX_ALIGN_CENTER, 
        "这是集合了一些小工具的软件，使用C语言写成。\n其中有图片、文件等相关的小工具，比如可以将图片转化成PDF。\n", gs->textColor);
    return aboutWidget;
}