#ifndef __HDR_stage_h
#define __HDR_stage_h


#include "includes.h"
#include "globals.h"
#include "scene.h"
#include "sprites.h"
#include "stagefx.h"

enum SLBLENDTYPE { SLB_DEFAULT=0, SLB_ONE };

#define SLINI_LIST \
SLPD(int, xres, 256) \
SLPD(int, yres, 256) \
SLPD(int, players, 255) \
SLPD(string, texture, "") \
SLPD(mcolor, color, C_WHITE) \
SLPD(GLfloat, xscale, 1) \
SLPD(GLfloat, scale, 1) \
SLPD(GLfloat, yscale, 1) \
SLPD(GLfloat, xpos, 0) \
SLPD(GLfloat, ypos, 0) \
SLPD(GLfloat, xcenter, 0) \
SLPD(GLfloat, ycenter, 0) \
SLPD(GLfloat, angle, 0) \
SLPD(SLBLENDTYPE, src_blending, SLB_DEFAULT) \
SLPD(SLBLENDTYPE, dst_blending, SLB_DEFAULT) \
SLPD(int, foreground, 0)

#define SLPD_DECLARE(a,b,c) a lv_##b;
#define SLPD_INIT(a,b,c) lv_##b = c;
#define SLPD_READ(a,b,c) if (tsimatch(line,"" #b)) { CONFREAD_##a(line,lv_##b); return; }

class Stage;

class StageLayer
{
public:
	string name;
	Stage *parent;
	int texid;
	mcolor color;
	int isBackground;
	vector<StageLayerFx*> fx;
	StageLayer();
	void read(char *line);
	void render();
#define SLPD SLPD_DECLARE
	SLINI_LIST
#undef SLPD
};

class Stage
{
public:
	vector<StageLayer*> layer;
	void readDir(string d);
	void cleanup();
	void render();
	void renderForeground();
	StageLayer *findLayer(string name);
	StageLayerFx *findFX(StageLayer *base, string name);
	~Stage() { cleanup();}
};


#endif
