#include "./widgetmanager.h"
#include "../defines.h"

OTS_WidgetManager *OTS_WidgetManager_Initialize(int width, int height) {
    OTS_WidgetManager *manager = (OTS_WidgetManager *)MP_Malloc(mp, sizeof(OTS_WidgetManager));
    OTS_printf("OTS_WidgetManager object have memory, address is %p.\n", manager);
    manager->widgets = OTS_Vector_Initialize(20);
    // int vflag = PX_VectorInitialize(mp, manager->widgets, sizeof(PX_Object), 10);
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
    if (!manager->widgets) {
        OTS_err_info("OTS_WidgetManger_AddWidget: OTS_WidgetManager::widgets is null.\n");
        return PX_FALSE;
    }
    px_bool success = OTS_Vector_Pushback(manager->widgets, widget);
    PX_Object_WidgetHide(widget);   // 默认新添加的窗口不显示
    if (PX_VectorSize(manager->widgets)==1) {   // 如果现在添加的窗口只有一个，那么显示这个窗口
        manager->nowWidget = 0;
        PX_Object_WidgetShow(widget); PX_Object_WidgetHide(manager->selfWidget);
    }
    OTS_printf("%s: new add widget address is %p.\n", __func__, widget);
    if (!success) {
        return -1;
    }
    return PX_VectorSize(manager->widgets)-1;
}

px_bool OTS_WidgetManager_WidgetShowByIndex(OTS_WidgetManager *manager, int index) {
    if (!manager||!manager->widgets) {
        OTS_err_info("manager or manager widget vector ptr is null.\n");
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
    // oldWidget = PX_VECTORAT(PX_Object, manager->widgets, manager->nowWidget);
    oldWidget = (PX_Object *)OTS_Vector_AT(manager->widgets, manager->nowWidget);
    PX_Object_WidgetHide(oldWidget); manager->nowWidget = index;
    // newWidget = PX_VECTORAT(PX_Object, manager->widgets, manager->nowWidget);
    newWidget = (PX_Object *)OTS_Vector_AT(manager->widgets, manager->nowWidget);
    PX_Object_WidgetShow(newWidget);
}

px_bool OTS_WidgetManager_WidgetShowByObject(OTS_WidgetManager *manager, PX_Object *widget) {
    if (!manager||!widget) {
        OTS_err_info("%s: OTS_WidgetManager object or PX_Object object is null.\n", __func__);
        return PX_FALSE;
    }
    OTS_printf("%s: OTS_WidgetManager manager widget number: %d.\n", __func__, PX_VectorSize(manager->widgets));
    int find = PX_FALSE;
    PX_Object *findWidget=NULL, *oldWidget=NULL;
    for (int i=0;i<PX_VectorSize(manager->widgets);i++) {
        // findWidget = PX_VECTORAT(PX_Object, manager->widgets, i);
        findWidget = (PX_Object *)OTS_Vector_AT(manager->widgets, i);
        if (memcmp(findWidget, widget, sizeof(PX_Object)) == 0) {
            find = PX_TRUE; 
            oldWidget = (PX_Object *)OTS_Vector_AT(manager->widgets, manager->nowWidget);
            PX_Object_WidgetHide(oldWidget); manager->nowWidget = i; break;
        }
    }
    if (!find) {
        OTS_err_info("%s: Cannot find widget, address is %p.\n", __func__, widget);
        return PX_FALSE;
    }
    PX_Object_WidgetShow(widget);
    OTS_printf("%s: find widget success, now switch to widget.\n", __func__);
    return find;
}