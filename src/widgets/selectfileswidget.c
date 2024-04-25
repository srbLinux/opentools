#include "selectfileswidget.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../defines.h"
#include "../global.h"

#include "../kernel/font.h"
#include "../kernel/debug.h"

typedef struct __OTS_SelectFilesWidgetItem {
    char *fileName;
    size_t fileSize;
    PX_Object *widget;
} OTS_SelectFilesWidgetItem;


struct OTS_SelectFilesWidget_Private {
    OTS_Vector *items; // Contain PX_Object object.
    OTS_SelectFilesWidget_AddItemEvent event;
    PX_Object *scrollArea, *selectBtn, *restartBtn, *explorer;
};

static int explorerGetPathFileCount(const char *path, const char *filter);
static int explorerGetPathFolderCount(const char *path, const char *filter);
static void selectFileCallback(PX_Object *obj, PX_Object_Event event, void *data);
static void restartFileCallback(PX_Object *obj, PX_Object_Event event, void *data);
static void deleteFileItemCallback(PX_Object *obj, PX_Object_Event event, void *data);
static void getSelectedPathCallback(PX_Object *obj, PX_Object_Event event, void *data);
static void closeSelectFilesWidgetCallback(PX_Object *obj, PX_Object_Event event, void *data);
static int explorerGetPathFileName(const char path[], int count, char filename[][260], const char *filter);
static int explorerGetPathFolderName(const char path[], int count, char filename[][260], const char *filter);

static OTS_SelectFilesWidgetItem *OTS_SelectFilesWidgetItem_Initialize(PX_Object *parent, int x, int y, int width, int height, const char *filepath, OTS_SelectFilesWidget *selectWidget);
static void OTS_SelectFilesWidgetItem_SetFile(OTS_SelectFilesWidgetItem *item, const char *filepath, size_t filesize);

OTS_SelectFilesWidget *OTS_SelectFilesWidget_Initialize(int width, int height, const char *name, const char *filter) {
    OTS_SelectFilesWidget *filesWidget = (OTS_SelectFilesWidget *)MP_Malloc(mp, sizeof(OTS_SelectFilesWidget));
    filesWidget->widget = OTS_DEFUALT_WIDGET_CREATE; filesWidget->width = width, filesWidget->height = height;
    PX_Object_WidgetShow(filesWidget->widget);
    filesWidget->filter = (char *)MP_Malloc(mp, (strlen(filter)+10)*sizeof(char));
    strcpy(filesWidget->filter, filter); filesWidget->texts = OTS_Vector_Initialize(20);
    filesWidget->data = (struct OTS_SelectFilesWidget_Private *)MP_Malloc(mp, sizeof(struct OTS_SelectFilesWidget_Private));
    filesWidget->data->items = OTS_Vector_Initialize(10);
    const char *selectText = "Select Files", *restartText = "Restart Selected Files";
    int btnWidth1 = OTS_FontTextWidth(gs->textFont, selectText)+10, btnHeight1 = OTS_FontTextHeight(gs->textFont, selectText)+10;
    filesWidget->data->selectBtn = PX_Object_PushButtonCreate(mp, filesWidget->widget, 15, 10, btnWidth1, btnHeight1, selectText, 
                                                                                                                gs->textFont->fontModule);
    int btnWidth2 = OTS_FontTextWidth(gs->textFont, restartText)+10, btnHeight2 = OTS_FontTextHeight(gs->textFont, restartText)+10;
    filesWidget->data->restartBtn = PX_Object_PushButtonCreate(mp, filesWidget->widget, 30 + btnWidth1, 10, btnWidth2, btnHeight2, 
                                                                                                    restartText, gs->textFont->fontModule);
    PX_Object *label = PX_Object_LabelCreate(mp, filesWidget->widget, 35, 20+btnHeight1+10, 100, 
                                                                btnHeight1, "File name", gs->textFont->fontModule, gs->textColor);
    label = PX_Object_LabelCreate(mp, filesWidget->widget, 35+500, 20+btnHeight1+10, 100, btnHeight1, 
                                                                            "File Size", gs->textFont->fontModule, gs->textColor);
    label = PX_Object_LabelCreate(mp, filesWidget->widget, 35+780, 20+btnHeight1+10, 200, btnHeight1, 
                                                                    "Item Operation", gs->textFont->fontModule, gs->textColor);
    filesWidget->data->scrollArea = PX_Object_ScrollAreaCreate(mp, filesWidget->widget, 15, 40+btnHeight1*2, width-20, height - 130 - btnHeight1);
    PX_Object_PushButtonSetStyle(filesWidget->data->selectBtn, PX_OBJECT_PUSHBUTTON_STYLE_ROUNDRECT);
    PX_Object_PushButtonSetStyle(filesWidget->data->restartBtn, PX_OBJECT_PUSHBUTTON_STYLE_ROUNDRECT);
    PX_ObjectRegisterEvent(filesWidget->data->selectBtn, PX_OBJECT_EVENT_EXECUTE, selectFileCallback, filesWidget);
    PX_ObjectRegisterEvent(filesWidget->data->restartBtn, PX_OBJECT_EVENT_EXECUTE, restartFileCallback, filesWidget);
    return filesWidget;
}

static char *gfilter = NULL;

void OTS_SelectFilesWidget_SetFilter(OTS_SelectFilesWidget *widget, const char *filter) {
    strcpy(widget->filter, filter);
}

void OTS_SelectFilesWidget_RegisterAddItemEvent(OTS_SelectFilesWidget *widget, OTS_SelectFilesWidget_AddItemEvent event) {
    widget->data->event = event;
    OTS_DEBUG("AAA\n");
    PX_ObjectRegisterEvent(widget->widget, PX_OBJECT_EVENT_WINDOWRESIZE, closeSelectFilesWidgetCallback, widget);
}

const char *OTS_SelectFilesWidget_GetFilter(OTS_SelectFilesWidget *widget) {
    return widget->filter;
}

OTS_Vector *OTS_SelectFilesWidget_GetFilesPath(OTS_SelectFilesWidget *widget) {
    return widget->texts;
}

void OTS_SelectFilesWidget_SetFilesPath(OTS_SelectFilesWidget *widget, const char **filespath, int fileslen) {
    for (int i=0;i<fileslen;i++) {
        char *filespath_copy = (char *)malloc(sizeof(char)*strlen(filespath[i]));
        strcpy(filespath_copy, filespath[i]);
        OTS_Vector_Pushback(widget->texts, filespath_copy);
        OTS_SelectFilesWidgetItem *item 
            = OTS_SelectFilesWidgetItem_Initialize(widget->data->scrollArea, 10, i*80+20, widget->width-20, 40, filespath[i], widget);
    }
}

void OTS_SelectFilesWidget_Free(OTS_SelectFilesWidget *widget) {
    OTS_Vector_Free(widget->texts, 1); OTS_Vector_Free(widget->data->items, 1);
    free(widget->filter); widget->data->restartBtn->Func_ObjectFree(widget->data->restartBtn);
}

void closeSelectFilesWidgetCallback(PX_Object *obj, PX_Object_Event event, void *data) {
    OTS_DEBUG("CLOSE %d\n", 1);
}

void selectFileCallback(PX_Object *obj, PX_Object_Event event, void *data) {
    OTS_SelectFilesWidget *selectWidget = (OTS_SelectFilesWidget *)data;
#if __DEBUG_OTS__
    PX_Object_Explorer *explorer = PX_Object_ExplorerCreate(mp, root, 0, WINDOW_DEFAULT_WIDTH - selectWidget->width,
                     WINDOW_DEFAULT_WIDTH, selectWidget->height-30, gs->textFont->fontModule, 
                     explorerGetPathFolderCount, explorerGetPathFileCount, 
                     explorerGetPathFolderName, explorerGetPathFileName, "/home/linux2024/opentools/resources/icons");
#else
    PX_Object_Explorer *explorer = PX_Object_ExplorerCreate(mp, root, 0, WINDOW_DEFAULT_WIDTH - selectWidget->width,
                     WINDOW_DEFAULT_WIDTH, selectWidget->height-30, gs->textFont->fontModule, 
                     explorerGetPathFolderCount, explorerGetPathFileCount, 
                     explorerGetPathFolderName, explorerGetPathFileName, "./");
#endif
    // OTS_FileSelect_Callback *callback = (OTS_FileSelect_Callback *)MP_Malloc(mp, sizeof(OTS_FileSelect_Callback));
    PX_ObjectRegisterEvent(explorer, PX_OBJECT_EVENT_EXECUTE, getSelectedPathCallback, selectWidget);
    if (strstr(selectWidget->filter, "png")) {
        gfilter = "png\0jpg\0jpeg\0svg\0";
    }
    PX_Object_ExplorerOpen(explorer);
}


void restartFileCallback(PX_Object *obj, PX_Object_Event event, void *data) {

}

void getSelectedPathCallback(PX_Object *obj, PX_Object_Event event, void *data) {
    OTS_DEBUG("data is not null, check %d.\n", (data!=NULL));
    // OTS_FileSelect_Callback *callback = (OTS_FileSelect_Callback *)data;
    OTS_SelectFilesWidget *widget = (OTS_SelectFilesWidget *)data;
    char filepath[512] = {0};
    PX_Object_ExplorerGetPath(obj, filepath, 0);
    // if (!filepath) return;
    OTS_DEBUG("Get file or folder path is not null, check %d.\n", __func__, (filepath!=NULL));
    OTS_DEBUG("Get File or Folder path is %s.\n", __func__, filepath);
    int size = OTS_Vector_Size(widget->texts);
    OTS_SelectFilesWidgetItem *item = 
        OTS_SelectFilesWidgetItem_Initialize(widget->data->scrollArea, 0, (size)*80, widget->width-20, 80, filepath, widget);
    OTS_Vector_Pushback(widget->texts, filepath); OTS_Vector_Pushback(widget->data->items, item);
    PX_Object_ExplorerClose(obj); obj->Func_ObjectFree(obj);
}

int explorerGetPathFolderCount(const char *path, const char *filter) {
    return PX_FileGetDirectoryFileCount(path, PX_FILEENUM_TYPE_FOLDER, filter);
}

int explorerGetPathFileCount(const char *path, const char *filter) {
    return PX_FileGetDirectoryFileCount(path, PX_FILEENUM_TYPE_FILE, gfilter);
}

int explorerGetPathFolderName(const char path[], int count, char filename[][260], const char *filter) {
    return PX_FileGetDirectoryFileName(path, count, filename, PX_FILEENUM_TYPE_FOLDER, filter);
}

int explorerGetPathFileName(const char path[], int count, char filename[][260], const char *filter) {
    return PX_FileGetDirectoryFileName(path, count, filename, PX_FILEENUM_TYPE_FILE, gfilter);
}

OTS_SelectFilesWidgetItem *OTS_SelectFilesWidgetItem_Initialize(PX_Object *parent, int x, int y, int width, int height, const char *filepath, OTS_SelectFilesWidget *selectWidget) {
    OTS_SelectFilesWidgetItem *item = (OTS_SelectFilesWidgetItem *)MP_Malloc(mp, sizeof(OTS_SelectFilesWidgetItem));
    item->widget = PX_Object_WidgetCreate(mp, parent, x, y, width, height, "", gs->textFont->fontModule);
    char *filepath_copy = (char *)malloc(sizeof(char)*strlen(filepath)); strcpy(filepath_copy, filepath);
    char *lastname=NULL, *last_component=NULL;
#if __linux__
    lastname = strtok(filepath_copy, "/");
    while (lastname) {
        last_component = lastname;
        lastname = strtok(NULL, "/");
    }
#elif _WIN32
    lastname = strtok(filepath_copy, "\\");
    while (lastname) {
        last_component = lastname;
        lastname = strtok(NULL, "\\");
    }
#endif
    int textWidth = OTS_FontTextWidth(gs->textFont, last_component)+10, textHeight = OTS_FontTextHeight(gs->textFont, last_component)+6;
    textWidth = 500, textHeight = 30;
    PX_Object *subwidget = PX_Object_LabelCreate(mp, item->widget, 20, 10, textWidth, textHeight, last_component, gs->textFont->fontModule, gs->textColor);
    FILE *pfile = fopen(filepath, "r");
    if (fseek(pfile, 0, SEEK_END) != 0) {
        fprintf(stderr, "%s: Unable to seek to end of file.\n", __func__); return NULL;
    }
    size_t filesize = ftell(pfile);
    char *filesize_str=(char *)malloc(sizeof(char)*20);
    sprintf(filesize_str, "%.1f KB", ((float)filesize/1024));
    int sizeWidth = OTS_FontTextWidth(gs->textFont, filesize_str)+10, sizeHeight = OTS_FontTextHeight(gs->textFont, filesize_str)+6;
    sizeWidth = 200, sizeHeight = 30;
    subwidget = PX_Object_LabelCreate(mp, item->widget, 20+textWidth+20, 10, sizeWidth, sizeHeight, filesize_str, gs->textFont->fontModule, gs->textColor);
    int btnWidth = OTS_FontTextWidth(gs->textFont, "Delete Item"), btnHeight = OTS_FontTextHeight(gs->textFont, "Delete Item")+6;
    subwidget = PX_Object_PushButtonCreate(mp, item->widget, width-btnWidth-50, 10, btnWidth+20, btnHeight, "Delete Item", gs->textFont->fontModule);
    PX_Object_PushButtonSetStyle(subwidget, PX_OBJECT_PUSHBUTTON_STYLE_ROUNDRECT);
    PX_ObjectRegisterEvent(subwidget, PX_OBJECT_EVENT_EXECUTE, deleteFileItemCallback, selectWidget);
    return item;
}

void deleteFileItemCallback(PX_Object *obj, PX_Object_Event event, void *data) {
    // PX_Object_WidgetHide(obj);

}