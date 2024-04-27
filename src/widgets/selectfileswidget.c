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
    int x, y, width, height;
} OTS_SelectFilesWidgetItem;


struct OTS_SelectFilesWidget_Private {
    OTS_Vector *items; // Contain PX_Object object.
    void *changeUsr, *deleteUsr, *addUsr;
    PX_Object *scrollArea, *selectBtn, *restartBtn, *explorer;
    OTS_SelectFilesWidget_Event changeEvent, addEvent, deleteEvent;
};

static int explorerGetPathFileCount(const char *path, const char *filter);
static int explorerGetPathFolderCount(const char *path, const char *filter);
static void selectFileCallback(PX_Object *obj, PX_Object_Event event, void *data);
static void restartFileCallback(PX_Object *obj, PX_Object_Event event, void *data);
static void deleteFileItemCallback(PX_Object *obj, PX_Object_Event event, void *data);
static void getSelectedPathCallback(PX_Object *obj, PX_Object_Event event, void *data);
static int explorerGetPathFileName(const char path[], int count, char filename[][260], const char *filter);
static int explorerGetPathFolderName(const char path[], int count, char filename[][260], const char *filter);

/**
 * OTS_SelectFilesWidgetItem 相关结构体函数
*/
static OTS_SelectFilesWidgetItem *OTS_SelectFilesWidgetItem_Initialize(PX_Object *parent, int x, int y, int width, int height, const char *filepath, OTS_SelectFilesWidget *selectWidget);
static void OTS_SelectFilesWidgetItem_Free(OTS_SelectFilesWidgetItem *item);

OTS_SelectFilesWidget *OTS_SelectFilesWidget_Initialize(int width, int height, const char *name, const char *filter) {
    OTS_SelectFilesWidget *filesWidget = (OTS_SelectFilesWidget *)MP_Malloc(mp, sizeof(OTS_SelectFilesWidget));
    filesWidget->widget = OTS_DEFUALT_WIDGET_CREATE; 
    filesWidget->width = width, filesWidget->height = height;
    PX_Object_WidgetShow(filesWidget->widget);
    filesWidget->filter = (char *)MP_Malloc(mp, (strlen(filter)+10)*sizeof(char));
    strcpy(filesWidget->filter, filter); 
    filesWidget->texts = OTS_Vector_Initialize(20);
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

/**
 * 三个注册回调的函数
*/
void OTS_SelectFilesWidget_RegisterItemAddEvent(OTS_SelectFilesWidget *widget, OTS_SelectFilesWidget_Event event, void *uptr) {
    widget->data->addEvent = event; widget->data->addUsr = uptr;
}

void OTS_SelectFilesWidget_RegisterItemChangeEvent(OTS_SelectFilesWidget *widget, OTS_SelectFilesWidget_Event event, void *uptr) {
    widget->data->changeEvent = event; widget->data->changeUsr = uptr;
}

void OTS_SelectFilesWidget_RegisterItemDeleteEvent(OTS_SelectFilesWidget *widget, OTS_SelectFilesWidget_Event event, void *uptr) {
    widget->data->deleteEvent = event; widget->data->deleteUsr = uptr;
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
        OTS_Vector_Pushback(widget->data->items, item); OTS_Vector_Pushback(widget->texts, filespath_copy);
    }
}

void OTS_SelectFilesWidget_Free(OTS_SelectFilesWidget *widget) {
    OTS_Vector_Free(widget->texts, 1); OTS_Vector_Free(widget->data->items, 1);
    free(widget->filter); widget->data->restartBtn->Func_ObjectFree(widget->data->restartBtn);
}

/**
 * 点击选择文件的函数回调
*/
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
    PX_ObjectRegisterEvent(explorer, PX_OBJECT_EVENT_EXECUTE, getSelectedPathCallback, selectWidget);
    if (strstr(selectWidget->filter, "png")) {
        gfilter = "png\0jpg\0jpeg\0svg\0";
    } else if (strstr(selectWidget->filter, "pdf")) {
        gfilter = "pdf\0";
    } else {
        gfilter = "";
    }
    PX_Object_ExplorerOpen(explorer);
}


void restartFileCallback(PX_Object *obj, PX_Object_Event event, void *data) {

}

/**
 * 选择之后获得选中文件路径的函数回调
 */
void getSelectedPathCallback(PX_Object *obj, PX_Object_Event event, void *data) {
    OTS_DEBUG("data is not null, check %d.\n", (data!=NULL));
    OTS_SelectFilesWidget *widget = (OTS_SelectFilesWidget *)data;
    char filepath[512] = {0};
    PX_Object_ExplorerGetPath(obj, filepath, 0);
    OTS_DEBUG("Get file or folder path is not null, check %d.\n", __func__, (filepath!=NULL));
    OTS_DEBUG("Get File or Folder path is %s.\n", __func__, filepath);
    int size = OTS_Vector_Size(widget->texts);
    OTS_SelectFilesWidgetItem *item = 
        OTS_SelectFilesWidgetItem_Initialize(widget->data->scrollArea, 0, (size)*80, widget->width-20, 80, filepath, widget);
    item->x = 0, item->y = size*80, item->width = widget->width-20, item->height = 80;
    OTS_Vector_Pushback(widget->texts, filepath); OTS_Vector_Pushback(widget->data->items, item);
    PX_Object_ExplorerClose(obj); obj->Func_ObjectFree(obj);
    if (widget->data->changeEvent) {
        widget->data->changeEvent(widget->data->changeUsr, widget->texts);
    }
    if (widget->data->addEvent) {
        widget->data->addEvent(widget->data->addUsr, filepath);
    }
}

struct DeleteFileItem {
    OTS_SelectFilesWidget *widget;
    OTS_SelectFilesWidgetItem *item;
};

//////////////////   OTS_SelectFilesWidgetItem 相关函数   ////////////////////

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
    struct DeleteFileItem *dItem = (struct DeleteFileItem *)MP_Malloc(mp, sizeof(struct DeleteFileItem));
    dItem->item = item; dItem->widget = selectWidget;
    PX_ObjectRegisterEvent(subwidget, PX_OBJECT_EVENT_EXECUTE, deleteFileItemCallback, dItem);
    return item;
}

/**
 * 绑定OTS_SelectFilesWidgetItem删除按钮的回调函数，即删除这个文件窗口。
 * 同时删除之后需要将此后的窗口的向上移动
*/
void deleteFileItemCallback(PX_Object *obj, PX_Object_Event event, void *uptr) {
    struct DeleteFileItem *item = (struct DeleteFileItem *)uptr;
    OTS_SelectFilesWidget *widget = item->widget;
    OTS_SelectFilesWidgetItem *fitem = item->item;
    int ii;
    for (ii=0;ii<OTS_Vector_Size(widget->data->items);ii++) {
        OTS_SelectFilesWidgetItem *newItem = OTS_Vector_AT(widget->data->items, ii);
        if (newItem==fitem) {
            break;
        }
    }
    char *text = OTS_Vector_Erase(widget->texts, ii);
    OTS_SelectFilesWidgetItem *iitem = OTS_Vector_Erase(widget->data->items, ii);
    free(text); OTS_SelectFilesWidgetItem_Free(iitem);
    // PX_Object_ScrollAreaMoveToBottom(fitem->widget);
    PX_Object_WidgetHide(fitem->widget); 
    OTS_SelectFilesWidgetItem_Free(fitem);
    for (;ii<OTS_Vector_Size(widget->data->items);ii++) {
        OTS_SelectFilesWidgetItem *moveItem = OTS_Vector_AT(widget->data->items, ii);
        // PX_Object_Widget
    }
}

void OTS_SelectFilesWidgetItem_Free(OTS_SelectFilesWidgetItem *item) {
    item->widget->Func_ObjectFree(item->widget);
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
