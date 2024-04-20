#ifndef _OPENTOOLS_DEFINES_H
#define _OPENTOOLS_DEFINES_H

#define WINDOW_DEFAULT_WIDTH   1000
#define WINDOW_DEFAULT_HEIGHT   600

#define OTS_DEFUALT_WIDGET_CREATE \
    PX_Object_WidgetCreate(mp, root, 0, WINDOW_DEFAULT_WIDTH - width, WINDOW_DEFAULT_WIDTH, height-30, "", NULL);

#include <stdio.h>

#if __DEBUG_OTS__
#define OTS_printf printf
#else
#define OTS_printf(str, ...)
#endif

#define OTS_err_info printf

#endif