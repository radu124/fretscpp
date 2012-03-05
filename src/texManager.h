#ifndef __HDR_texManager_h
#define __HDR_texManager_h


#include "includes.h"
#include "sprites.h"
/*
 * This avoids loading the same texture multiple times and handles
 * context loss gracefully on Windows
 * */

int texLoad(const char *c);
int texLoad(string s, int flags=0);
void texRelease(int i);
void texBind(int i);
void texUnbind();
void texDraw(int i);
void texReleaseAll();
void texReloadAll();
GLfloat texAspect(int i);


#endif
