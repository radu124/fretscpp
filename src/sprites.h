#ifndef __HDR_sprites_h
#define __HDR_sprites_h


#include "includes.h"
#include "utils.h"

void init_sprites();

class sprite
{
public:
	void load(const char *, int flags=0);
	void loadInternal(string, int flags=0);
	void setCenter(GLfloat, GLfloat);
	void bind();
	int width, height;
	int refcount;
	string name;
	GLuint tex;
	GLfloat scale;
	GLfloat txwi,txhe;
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
