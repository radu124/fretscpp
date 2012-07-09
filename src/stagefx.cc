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
		glTranslatef(cos(t/2)*16, sin(t)*16, 0);
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
		glTranslatef(v*v*80, 0, 0);
	}
};

class tFXwiggle:public tStageFx
{
public:
	void apply(tStageElem *el)
	{
		GLfloat v=sin(fx_drive->val()*1.57);
		glTranslatef(sin(6.28*v)*fx_xmagnitude->val()*el->lv_xscale*10,cos(6.28*v)*fx_ymagnitude->val()*el->lv_yscale*10,0);
	}
};

class tFXscale:public tStageFx
{
public:
	void apply(tStageElem *el)
	{
		glScalef(fx_xmagnitude->val(),fx_ymagnitude->val(),1);
	}
};

class tFXzoom:public tStageFx
{
public:
	void apply(tStageElem *el)
	{
		GLfloat x=fx_xmagnitude->val();
		glScalef(x,x,x);
	}
};

class tFXrotate:public tStageFx
{
public:
	void apply(tStageElem *el)
	{
		glRotatef(fx_angle->val(),0.0,0.0,1.0);
	}
};

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

