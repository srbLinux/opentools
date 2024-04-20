#include "mainwindow.h"

#include "../global.h"
#include "../defines.h"
#include "./widgetmanager.h"
#include "../kernel/font.h"
#include "../kernel/style.h"

// widgets headers
#include "./imagetopdfwidget.h"
#include "./aboutsoftwarewidget.h"

#ifdef __DEBUG_OPEN__
#include <stdio.h>
#endif

OTS_Style *gs = NULL;

typedef struct MainWindow {
    PX_Object *menuWidget;
    OTS_WidgetManager *managerWidget;
    PX_Object_Menu_Item *parentItem, *childItem;
    PX_Object *aboutWidget, *toPDFWidget, *toImageWidget;
} MainWindow;

static void SettingWidget(px_void *ptr);
static void ArchiveWidget(px_void *ptr);
static void ImageToPDFWidget(px_void *ptr);
static void PDFToImageWidget(px_void *ptr);
static void AboutSoftwareWidget(px_void *ptr);
static void MainWindowChangeByGS(px_void *data);

void OTS_MainWindow_Initialize() {
    PainterEngine_Initialize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
    gs = OTS_Style_Software_Create(SOFTWARE_DARK);
    MainWindow *mainWidget=(MainWindow *)MP_Malloc(mp, sizeof(MainWindow));
    OTS_printf("MainwWindow object initialize, check %d.\n", (mainWidget!=NULL));
    mainWidget->menuWidget = PX_Object_MenuCreate(mp, root, 0, 0, WINDOW_DEFAULT_WIDTH/4, gs->textFont->fontModule);
    mainWidget->managerWidget = OTS_WidgetManager_Initialize(WINDOW_DEFAULT_WIDTH-30, WINDOW_DEFAULT_HEIGHT);
    mainWidget->parentItem = PX_Object_MenuAddItem(mainWidget->menuWidget, NULL, "File Operation", NULL, NULL);
    mainWidget->childItem = PX_Object_MenuAddItem(mainWidget->menuWidget, mainWidget->parentItem, "File Compression", ArchiveWidget, mainWidget);

    mainWidget->parentItem = PX_Object_MenuAddItem(mainWidget->menuWidget, NULL, "Image Operation", NULL, NULL);
    mainWidget->childItem = PX_Object_MenuAddItem(mainWidget->menuWidget, mainWidget->parentItem, "Image To PDF", ImageToPDFWidget, mainWidget);
    mainWidget->childItem = PX_Object_MenuAddItem(mainWidget->menuWidget, mainWidget->parentItem, "PDF To Image", PDFToImageWidget, mainWidget);

    mainWidget->parentItem = PX_Object_MenuAddItem(mainWidget->menuWidget, NULL, "About", NULL, NULL);
    mainWidget->childItem = PX_Object_MenuAddItem(mainWidget->menuWidget, mainWidget->parentItem, "Software Settings", SettingWidget, mainWidget);
    mainWidget->childItem = PX_Object_MenuAddItem(mainWidget->menuWidget, mainWidget->parentItem, "About Software", AboutSoftwareWidget, mainWidget);

    /***    添加子窗口    ***/
    OTS_AboutSoftwareWidget *ots_aboutSWWidget = OTS_AboutSoftwareWidget_Initialize(WINDOW_DEFAULT_WIDTH-30, WINDOW_DEFAULT_HEIGHT);
    mainWidget->aboutWidget = ots_aboutSWWidget->widget; OTS_WidgetManager_AddWidget(mainWidget->managerWidget, mainWidget->aboutWidget); 

    OTS_ImageToPDFWidget *ots_imagetopdfWidget = OTS_ImageToPDFWidget_Initialize(WINDOW_DEFAULT_WIDTH-30, WINDOW_DEFAULT_HEIGHT);
    mainWidget->toPDFWidget = ots_imagetopdfWidget->widget; OTS_WidgetManager_AddWidget(mainWidget->managerWidget, mainWidget->toPDFWidget);

    //
    OTS_Global_StyleChange(MainWindowChangeByGS, mainWidget);
    
}

void ArchiveWidget(px_void *ptr) {

}

void SettingWidget(px_void *ptr) {

}

void ImageToPDFWidget(px_void *ptr) {
    MainWindow *obj = (MainWindow *)ptr;
    OTS_WidgetManager_WidgetShowByIndex(obj->managerWidget, 1);
}

void PDFToImageWidget(px_void *ptr) {

}


void AboutSoftwareWidget(px_void *ptr) {
    MainWindow *obj = (MainWindow *)ptr;
    OTS_printf("all.\n");
    OTS_WidgetManager_WidgetShowByIndex(obj->managerWidget, 0);
}

void MainWindowChangeByGS(px_void *data) {

}