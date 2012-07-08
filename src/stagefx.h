#ifndef __HDR_stagefx_h
#define __HDR_stagefx_h


#include "includes.h"
#include "globals.h"
#include "scene.h"
#include "sprites.h"
#include "utils.h"
#include "confrw.h"
#include "message.h"
#include "verbosity.h"

enum FXTYPE    { FX_NONE=0, FX_SCALE, FX_LIGHT, FX_WIGGLE, FX_ROTOBACK, FX_JUMPINLEFT, FX_WOBBLE, FX_TVFADE, FX_ROTATE};
enum FXTRIGGER { FXT_NONE=0, FXT_PICK, FXT_MISS, FXT_BEAT };
enum FXPROFILE { FXP_NONE=0, FXP_STEP, FXP_LINSTEP, FXP_SMOOTHSTEP, FXP_SINSTEP };

#define FXINI_LIST \
FXPD(FXTRIGGER,trigger,FXT_NONE) \
FXPD(FXPROFILE,profile,FXP_NONE) \
FXPD(int,light_number,0) \
FXPD(GLfloat,xmagnitude,0.1) \
FXPD(GLfloat,ymagnitude,0.1) \
FXPD(GLfloat,ambient,0.5) \
FXPD(GLfloat,contrast,0.5) \
FXPD(GLfloat,delay,0) \
FXPD(GLfloat,frequency,6) \
FXPD(GLfloat,angle,0) \
FXPD(GLfloat,period,500) \
FXPD(GLfloat,intensity,1)

#define FXPD_DECLARE(a,b,c) a fx_##b;
#define FXPD_INIT(a,b,c) fx_##b = c;
#define FXPD_READ(a,b,c) if (tsimatch(line,"" #b)) { CONFREAD_##a(line,fx_##b); return; }

class tStageElem;

class tStageFx
{
public:
	FXTYPE fx_type;
	string name;
	void apply(tStageElem *el);
	float trigval();
	float trigprofiled();
	tStageFx();
	void read(char *line);
#define FXPD FXPD_DECLARE
	FXINI_LIST
#undef FXPD
};

tStageFx* createStageFx(string typ);

#endif
