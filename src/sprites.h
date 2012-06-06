#ifndef __HDR_sprites_h
#define __HDR_sprites_h


#include "includes.h"
#include "utils.h"

void init_sprites();

class vpxplayer;

class sprite
{
public:
	/**
	 * load from the default data directory
	 */
	void load(const char *, int flags=0);
	/**
	 * load a PNG or AVI from the given path
	 * @param fullpath filename with full path
	 *    if the name ends in .[aA][vV][iI] then the file is considered
	 *    to be an AVI, otherwise it is considered to be a PNG
	 * @param flags
	 */
	void loadInternal(string fullpath, int flags=0);
	/**
	 * bind this texture as the current GL texture
	 */
	void bind();
	/**
	 * release resources allocated to this texture
	 *  - tex (GL texture id), (set tex to 0)
	 *  - avi file if associated (set isavi to 0)
	 */
	void release();

	int width;
	int height;
	int refcount;
	int isavi;
	int boundframesago;
	vpxplayer* vp;
	string name;
	GLuint tex;
	GLfloat scale;
	GLfloat txwi,txhe;
	sprite():isavi(0),boundframesago(0) {;}
};

extern int sp_neck;
extern int sp_ball;
extern int sp_flame;
extern int sp_glow;
extern int sp_key[5];
extern int sp_note[5];
extern int sp_hitg1, sp_hitg2;
extern int sp_hitf1, sp_hitf2;
extern int sp_noteline, sp_thinline;
extern int sp_guitar[8];
extern int sp_star1, sp_star2;
extern int sp_m2, sp_m3, sp_m4;


#endif
