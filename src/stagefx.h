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
#include "numexpr.h"

enum FXTYPE    { FX_NONE=0, FX_SCALE, FX_LIGHT, FX_WIGGLE, FX_ROTOBACK, FX_JUMPINLEFT, FX_WOBBLE, FX_TVFADE, FX_ROTATE};
enum FXTRIGGER { FXT_NONE=0, FXT_PICK, FXT_MISS, FXT_BEAT };
enum FXPROFILE { FXP_NONE=0, FXP_STEP, FXP_LINSTEP, FXP_SMOOTHSTEP, FXP_SINSTEP };

#define FXINI_LIST \
FXPD(GLfloat,xmagnitude,0.1) \
FXPD(GLfloat,ymagnitude,0.1) \
FXPD(GLfloat,ambient,0.5) \
FXPD(GLfloat,contrast,0.5) \
FXPD(GLfloat,delay,0) \
FXPD(GLfloat,frequency,6) \
FXPD(GLfloat,angle,0) \
FXPD(GLfloat,period,500) \
FXPD(GLfloat,intensity,1) \
FXPD(GLfloat,drive,sin t)

#define FXPD_DECLARE(a,b,c) tNumExpr * fx_##b;
#define FXPD_INIT(a,b,c) fx_##b = parseNumExpression(#c);
#define FXPD_READ(a,b,c) if (tsimatch(line,"" #b)) { delete fx_##b; fx_##b=parseNumExpression(line); return; }
#define FXPD_DELETE(a,b,c) delete fx_##b;

class tStageElem;

class tStageFx
{
public:
	string name;
	virtual void apply(tStageElem *el);
	virtual void read(char *line);
	float trigval();
	float trigprofiled();
	tStageFx();
	~tStageFx();
#define FXPD FXPD_DECLARE
	FXINI_LIST
#undef FXPD
};

/**
 * Translate effect, default initializer uses stage coordinates (40x30)
 */
class tFXtranslate:public tStageFx
{
public:
	tFXtranslate();
	tFXtranslate (string x, string y);
	void apply(tStageElem *el);
};

/**
 * Scale effect
 */
class tFXscale:public tStageFx
{
public:
	tFXscale();
	tFXscale (string x, string y);
	void apply(tStageElem *el);
};

/**
 * Rotate in the screen plane effect
 */
class tFXrotate:public tStageFx
{
public:
	tFXrotate();
	tFXrotate (string x);
	void apply(tStageElem *el);
};

/**
 * Rotate in the screen plane effect
 */
class tFXzoom:public tStageFx
{
public:
	tFXzoom();
	tFXzoom (string x);
	void apply(tStageElem *el);
};

tStageFx* createStageFx(string typ);

#endif
