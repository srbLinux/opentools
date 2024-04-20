#include "./widgetmanager.h"
#include "../defines.h"

OTS_WidgetManager *OTS_WidgetManager_Initialize(int width, int height) {
    OTS_WidgetManager *manager = (OTS_WidgetManager *)MP_Malloc(mp, sizeof(OTS_WidgetManager));
    OTS_printf("OTS_WidgetManager object have memory, address is %p.\n", manager);
    manager->widgets = (px_vector *)MP_Malloc(mp, sizeof(px_vector));
    int vflag = PX_VectorInitialize(mp, manager->widgets, sizeof(PX_Object), 10);
    OTS_printf("OTS_WidgetManager::widgets is a vector object, it has been initialize, check %d.\n", (manager->widgets!=NULL));
    manager->width = width, manager->height = height; manager->nowWidget = -1;
    manager->selfWidget = PX_Object_WidgetCreate(mp, root, 0, WINDOW_DEFAULT_WIDTH - width, WINDOW_DEFAULT_WIDTH, height-30, "", NULL);
    OTS_printf("OTS_WidgetManager::selfWidget is a widget object, it has been initialize\n");
    return manager;
}

int OTS_WidgetManager_AddWidget(OTS_WidgetManager *manager, PX_Object *widget) {
    if (!widget) {
        OTS_printf("OTS_WidgetManager_AddWidget: PX_Object object is null.\n");
        return PX_FALSE;
    }
    if (!manager) {
        OTS_printf("OTS_WidgetManager_AddWidget: OTS_WidgetManager is null.\n");
        return PX_FALSE;
    }
    OTS_printf("OTS_WidgetManager_AddWidget: check OTS_WidgetManager object address %p.\n", manager);
    if (!manager->widgets) {
        OTS_printf("OTS_WidgetManger_AddWidget: OTS_WidgetManager::widgets is null.\n");
        return PX_FALSE;
    }
    px_bool success = PX_VectorPushback(manager->widgets, widget);
    PX_Object_WidgetHide(widget);   // 默认新添加的窗口不显示
    if (PX_VectorSize(manager->widgets)==1) {   // 如果现在添加的窗口只有一个，那么显示这个窗口
        manager->nowWidget = 0;
        PX_Object_WidgetShow(widget); PX_Object_WidgetHide(manager->selfWidget);
    }
    if (!success) {
        return -1;
    }
    return PX_VectorSize(manager->widgets)-1;
}

px_bool OTS_WidgetManager_WidgetShowByIndex(OTS_WidgetManager *manager, int index) {
    if (!manager||!manager->widgets) {
        OTS_printf("manager or manager widget vector ptr is null.\n");
        return PX_FALSE;
    }
    if (index < 0 || index > PX_VectorSize(manager->widgets)) {
        return PX_FALSE;
    }
    if (index == manager->nowWidget) {
        return PX_TRUE;
    }
    OTS_printf("%s: now manager has %d widget.\n", __func__, PX_VectorSize(manager->widgets));
    PX_Object *oldWidget=NULL, *newWidget=NULL;
    PX_VectorSet(manager->widgets, manager->nowWidget, oldWidget);
    PX_Object_WidgetHide(oldWidget); manager->nowWidget = index;
    PX_VectorSet(manager->widgets, manager->nowWidget, newWidget);
    PX_Object_WidgetShow(newWidget);
}

px_bool OTS_WidgetManager_WidgetShowByObject(OTS_WidgetManager *manager, PX_Object *widget) {
    if(root!=PX_Object_WidgetGetRoot(widget)) {
        return -1;
    }
    int find = PX_FALSE;
    PX_Object *findWidget=NULL, *oldWidget=NULL;
    for (int i=0;i<PX_VectorSize(manager->widgets);i++) {
        PX_VectorSet(manager->widgets, i, findWidget);
        if (findWidget==widget) {
            find = PX_TRUE; 
            PX_VectorSet(manager->widgets, manager->nowWidget, oldWidget);
            PX_Object_WidgetHide(oldWidget); manager->nowWidget = i; break;
        }
    }
    if (!find) {
        return -1;
    }
    PX_Object_WidgetShow(widget);
}