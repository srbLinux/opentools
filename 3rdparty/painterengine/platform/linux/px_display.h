#ifndef PX_DISPLAY_H
#define PX_DISPLAY_H
#include <GL/freeglut.h>

int  PX_CreateWindow( int surfaceWidth,int surfaceHeight,int windowWidth,int windowHeight, const char *name);
int  PX_GetScreenHeight();
int  PX_GetScreenWidth();
int  PX_SystemLoop();
#endif