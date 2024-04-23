#ifndef PX_EFFECT_H
#define PX_EFFECT_H
#include "PX_Surface.h"

px_bool PX_EffectShine(px_surface *s,px_int radius,px_color color,px_float intension);
px_bool PX_EffectOutline(px_surface *s,px_int radius,px_color color);
px_void PX_EffectRenderAsh(px_surface *s,px_float x,px_float y,px_float radius);


#endif
