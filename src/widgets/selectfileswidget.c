#include "selectfileswidget.h"

#include "../defines.h"
#include "../global.h"

#include "../kernel/font.h"

typedef struct __OTS_SelectFilesWidgetItem {
    char *fileName;
    size_t fileSize;
    PX_Object *label;
} OTS_SelectFilesWidgetItem;

static OTS_SelectFilesWidgetItem *OTS_SelectFilesWidgetItem_Initialize(int width, int height);
static void OTS_SelectFilesWidgetItem_SetFile(OTS_SelectFilesWidgetItem *item, const char *filepath, size_t filesize);

struct OTS_SelectFilesWidget_Private {
    OTS_Vector *items; // Contain PX_Object object.
    PX_Object *scrollArea, *selectBtn, *restartBtn;
};

static void selectFileCallback(PX_Object *obj, PX_Object_Event event, void *data);
static void restartFileCallback(PX_Object *obj, PX_Object_Event event, void *data);

OTS_SelectFilesWidget *OTS_SelectFilesWidget_Initialize(int width, int height, const char *name, const char *filter) {
    OTS_SelectFilesWidget *filesWidget = (OTS_SelectFilesWidget *)MP_Malloc(mp, sizeof(OTS_SelectFilesWidget));
    filesWidget->widget = OTS_DEFUALT_WIDGET_CREATE; filesWidget->width = width, filesWidget->height = height;
    PX_Object_WidgetShow(filesWidget->widget);
    filesWidget->filter = (char *)MP_Malloc(mp, (strlen(filter)+10)*sizeof(char));
    strcpy(filesWidget->filter, filter); filesWidget->texts = OTS_Vector_Initialize(20);
    filesWidget->data = (struct OTS_SelectFilesWidget_Private *)MP_Malloc(mp, sizeof(struct OTS_SelectFilesWidget_Private));
    filesWidget->data->items = (OTS_Vector *)MP_Malloc(mp, sizeof(OTS_Vector));
    const char *selectText = "Select Files", *restartText = "Restart Selected Files";
    int btnWidth1 = OTS_FontTextWidth(gs->textFont, selectText)+10, btnHeight1 = OTS_FontTextHeight(gs->textFont, selectText)+10;
    filesWidget->data->selectBtn = PX_Object_PushButtonCreate(mp, filesWidget->widget, 15, 10, btnWidth1, btnHeight1, selectText, 
                                                                                                                gs->textFont->fontModule);
    int btnWidth2 = OTS_FontTextWidth(gs->textFont, restartText)+10, btnHeight2 = OTS_FontTextHeight(gs->textFont, restartText)+10;
    filesWidget->data->restartBtn = PX_Object_PushButtonCreate(mp, filesWidget->widget, 30 + btnWidth1, 10, btnWidth2, btnHeight2, 
                                                                                                    restartText, gs->textFont->fontModule);
    filesWidget->data->scrollArea = PX_Object_ScrollAreaCreate(mp, filesWidget->widget, 15, 20+btnHeight1, width-20, height - 130);
    PX_Object_PushButtonSetStyle(filesWidget->data->selectBtn, PX_OBJECT_PUSHBUTTON_STYLE_ROUNDRECT);
    PX_Object_PushButtonSetStyle(filesWidget->data->restartBtn, PX_OBJECT_PUSHBUTTON_STYLE_ROUNDRECT);
    PX_ObjectRegisterEvent(filesWidget->data->selectBtn, PX_OBJECT_EVENT_EXECUTE, selectFileCallback, filesWidget);
    PX_ObjectRegisterEvent(filesWidget->data->restartBtn, PX_OBJECT_EVENT_EXECUTE, restartFileCallback, filesWidget);
    return filesWidget;
}

void selectFileCallback(PX_Object *obj, PX_Object_Event event, void *data) {
    OTS_SelectFilesWidget *selectWidget = (OTS_SelectFilesWidget *)data;
    PX_Object *explorer = PX_Object_ExplorerCreate(mp, root, 0, WINDOW_DEFAULT_WIDTH - selectWidget->width,
                     WINDOW_DEFAULT_WIDTH, selectWidget->height-30, gs->textFont->fontModule, NULL, NULL, NULL, NULL, "");
    PX_Object_ExplorerOpen(explorer);
}

void restartFileCallback(PX_Object *obj, PX_Object_Event event, void *data) {

}