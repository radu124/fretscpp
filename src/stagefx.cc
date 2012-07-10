/*******************************************************************
(C) 2011 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

#include "stagefx.h"
#include "globals.h"
#include "scnGuitar.h"
#include "stage.h"


tStageFx::tStageFx()
{
#define FXPD FXPD_INIT
	FXINI_LIST
#undef FXPD
}

tStageFx::~tStageFx()
{
#define FXPD FXPD_DELETE
	FXINI_LIST
#undef FXPD
}

void tStageFx::read(char *line)
{
#define FXPD FXPD_READ
	FXINI_LIST
#undef FXPD
	WARN(STAGEFX,"Unknown parameter %s\n",line);
}

class tFXrotoback:public tStageFx
{
public:
	void apply(tStageElem *el)
	{
		GLfloat t=scn.timesc*0.33;
		glTranslatef(cos(t/2)*1.6, sin(t)*1.6, 0);
		glRotatef(t*53.3f+30,0.0,0.0,1.0);
		GLfloat s=(2+sin(t/8))/2;
		glScalef(s, s, 1.0);
	}
};

class tFXwobble:public tStageFx
{
public:
	void apply(tStageElem *el)
	{
		GLfloat v=scn.fade*2;
		v=(v>1)?1:v*v;
		GLfloat t=scn.timesc*0.33;
		glScalef(1+0.025*sin(t*16), 1+0.025*sin(t*17),1.0);
	}
};

class tFXtvfade:public tStageFx
{
public:
	void apply(tStageElem *el)
	{
		GLfloat v=scn.fade*2;
		v=(v>1)?1:v*v;
		glScalef(1,1-v*v*v,1.0);
		if (v>0.95) el->color.Alpha=0;
	}
};

class tFXjumpinleft:public tStageFx
{
public:
	void apply(tStageElem *el)
	{
		GLfloat v=scn.fade*2;
		v=(v>1)?1:v*v;
		glTranslatef(v*v*8, 0, 0);
	}
};

class tFXwiggle:public tStageFx
{
public:
	void apply(tStageElem *el)
	{
		GLfloat v=sin(fx_drive->val()*1.57);
		glTranslatef(sin(6.28*v)*fx_xmagnitude->val(),cos(6.28*v)*fx_ymagnitude->val(),0);
	}
};

/*======================================================================
 * Scale effect
 */
tFXscale::tFXscale(){;}

tFXscale::tFXscale(string x, string y)
{
	delete fx_xmagnitude;
	delete fx_ymagnitude;
	fx_xmagnitude=parseNumExpression(x.c_str());
	fx_ymagnitude=parseNumExpression(y.c_str());
}

void tFXscale::apply(tStageElem *el)
{
	glScalef(fx_xmagnitude->val(),fx_ymagnitude->val(),1);
}


/*======================================================================
 * Translate effect
 */
tFXtranslate::tFXtranslate(){;}

tFXtranslate::tFXtranslate (string x, string y)
{
	delete fx_xmagnitude;
	delete fx_ymagnitude;
	fx_xmagnitude=parseNumExpression(x.c_str());
	fx_ymagnitude=parseNumExpression(y.c_str());
}

void tFXtranslate::apply(tStageElem *el)
{
	glTranslatef(fx_xmagnitude->val(),fx_ymagnitude->val(),0);
}

/*======================================================================
 * Rotate in the screen plane effect
 */

tFXrotate::tFXrotate(){;}

tFXrotate::tFXrotate(string x)
{
	delete fx_angle;
	fx_angle=parseNumExpression(x.c_str());
}


void tFXrotate::apply(tStageElem *el)
{
	glRotatef(fx_angle->val(),0.0,0.0,1.0);
}


/*======================================================================
 * Zoom effect, same as scale, but with equal values on all axes
 */

tFXzoom::tFXzoom() {;}

tFXzoom::tFXzoom(string x)
{
	delete fx_xmagnitude;
	fx_xmagnitude=parseNumExpression(x.c_str());
}

void tFXzoom::apply(tStageElem *el)
{
	GLfloat x=fx_xmagnitude->val();
	glScalef(x,x,x);
}

/*======================================================================
 * Light effect
 */
class tFXlight:public tStageFx
{
public:
	GLfloat fx_offbefore;
	tFXlight():fx_offbefore(0)
		{;}
	void read(char *line)
	{
		if (tsimatch(line,"offbefore")) { CONFREAD_GLfloat(line,fx_offbefore); return; }
		tStageFx::read(line);
	}
	void apply(tStageElem *el)
	{
		if (guitarScene.timenow<=fx_offbefore*44100)
		{
			el->color=C_TRANSPARENT;
			return;
		}
		GLfloat v=fx_drive->val()*0.9;
		if (v<0) v=0;
		else if (v<0.2) v=v*8;
		else if (v<1) v=2-v*2;
		else v=0;
		if (v<0.01) return;
		mcolor efcolor(1,1,0.8,0.8);
		el->color.weight(el->color,efcolor,v*fx_intensity->val());
	}
};


class tFXdummy:public tStageFx
{
public:
	void apply(tStageElem *el)
	{
	}
};

void tStageFx::apply(tStageElem *el) {};

tStageFx* createStageFx(string vtyp)
{
	if (vtyp=="scale")  { return new tFXscale(); }
	if (vtyp=="zoom")   { return new tFXzoom(); }
	if (vtyp=="rotate") { return new tFXrotate(); }
	if (vtyp=="translate") { return new tFXtranslate(); }
	if (vtyp=="light")  { return new tFXlight(); }

	if (vtyp=="rotoback") { return new tFXrotoback(); }
	if (vtyp=="jumpinleft") { return new tFXjumpinleft(); }
	if (vtyp=="wobble") { return new tFXwobble(); }
	if (vtyp=="wiggle") { return new tFXwiggle(); }
	if (vtyp=="tvfade") { return new tFXtvfade(); }
//	if (vtyp=="") { return new tFX(); }
	WARN(STAGEFX,"Unknown effect: %s\n",vtyp);
	return new tStageFx();
}

