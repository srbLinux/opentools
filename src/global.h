#ifndef _OPENTOOLS_GLOBAL_H_
#define _OPENTOOLS_GLOBAL_H_

#include "kernel/style.h"

extern OTS_Style *gs;

typedef px_void (*style_change_style_callback)(void *meta_data);

extern px_void OTS_Global_StyleChange(style_change_style_callback callback, void *meta_data);

#endif