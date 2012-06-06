#ifndef __HDR_texManager_h
#define __HDR_texManager_h


#include "includes.h"
#include "sprites.h"
/*
 * This avoids loading the same texture multiple times and handles
 * context loss gracefully on Windows
 * */

/**
 * load the texture with the given name
 * @param c file name
 */
int texLoad(const char *c);
/**
 * load the texture with the given name
 * @param s file name
 * @param flags
 *
 */
int texLoad(string s, int flags=0);
/**
 * decrease the reference count of the given texture
 * warn if the count becomes negative
 * currently does nothing when 0 is reached
 */
void texRelease(int i);

/**
 * bind the texture with id i
 */
void texBind(int i);

/**
 * glBindTexture(GL_TEXTURE_2D,NULL);
 */
void texUnbind();

/**
 * draw the given texture in a box
 * with the corners -1,-1 and 1,1
 * @param i, the id of the texture
 */
void texDraw(int i);

/**
 * call release() on all textures
 */
void texReleaseAll();

/**
 * reloads all textures that have a positive reference counter
 */
void texReloadAll();

/**
 * iterate through textures and for all AVI textures step frame
 * if that is the case
 */
void texStepAVIs();

GLfloat texAspect(int i);


#endif
