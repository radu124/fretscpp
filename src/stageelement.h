#ifndef __HDR_stageelement_h
#define __HDR_stageelement_h


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
SLPD(string, effects, "") \
SLPD(mcolor, color, C_WHITE) \
SLPD(string, xscale, "1") \
SLPD(string, scale, "1") \
SLPD(string, yscale, "1") \
SLPD(GLfloat, aspect, 0) \
SLPD(string, xpos, "0") \
SLPD(string, ypos, "0") \
SLPD(string, rotate, "0") \
SLPD(string, children, "") \
SLPD(SLBLENDTYPE, src_blending, SLB_DEFAULT) \
SLPD(SLBLENDTYPE, dst_blending, SLB_DEFAULT) \
SLPD(int, foreground, 0)

#define SLPD_DECLARE(a,b,c) a lv_##b;
#define SLPD_INIT(a,b,c) lv_##b = c;
#define SLPD_READ(a,b,c) if (tsimatch(line,"" #b)) { CONFREAD_##a(line,lv_##b); return; }

class Stage;

class tStageElem
{
public:
	GLfloat cropleft;
	GLfloat cropright;
	GLfloat croptop;
	GLfloat cropbtm;
	int cropmode;
	string name;
	vector<tStageElem*> children;
	int texid;
	mcolor color;
	int isBackground;
	vector<tStageFx*> fx;
	tStageElem();
	~tStageElem();
	void read(char *line);
	virtual void render(int depth=0);
	tNumExpr * lv_showif;
#define SLPD SLPD_DECLARE
	SLINI_LIST
#undef SLPD
};

class tStageBackground:public tStageElem
{
	void render(int depth=0);
};

#endif
