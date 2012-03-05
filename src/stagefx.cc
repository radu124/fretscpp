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

MULTIRDR(FXTYPE,"none,scale,light,wiggle,rotoback,jumpinleft,wobble,tvfade,rotate");
MULTIRDR(FXTRIGGER,"none,pick,miss,beat");
MULTIRDR(FXPROFILE,"none,step,linstep,smoothstep,sinstep");

StageLayerFx::StageLayerFx()
{
#define FXPD FXPD_INIT
	FXINI_LIST
#undef FXPD
}

void StageLayerFx::read(char *line)
{
#define FXPD FXPD_READ
	FXINI_LIST
#undef FXPD
}

float StageLayerFx::trigval()
{
	double temp;
	switch (fx_trigger)
	{
	case FXT_BEAT:
		return modf(guitarScene.timenow*0.000024*fx_frequency,&temp);
	case FXT_MISS:
		return (guitarScene.timenow-guitarScene.timelastmiss)*2.267e-02/fx_period;
	case FXT_PICK:
		return (guitarScene.timenow-guitarScene.timelasthit)*2.267e-02/fx_period;
	}
	return 0;
}

float StageLayerFx::trigprofiled()
{
	float tv=trigval();
	if (tv<0) tv=0;
	switch (fx_profile)
	{
	case FXP_NONE: return tv;
	case FXP_STEP: return tv<1?1:0;
	case FXP_LINSTEP: return tv<1?tv:1;
	case FXP_SMOOTHSTEP: return tv<1?0.5*(1-cos(tv*3.14)):1;
	case FXP_SINSTEP: return tv<1?sin(tv*3.14):0;
	}
}

float beatprofile(float x)
{
	if (x<0) return 0;
	if (x>1) return 0;
	if (x<0.2) return x*5;
	return 1.25-x*1.25;
}

void StageLayerFx::apply(mcolor &color)
{
	GLfloat t,s,v;
	switch (fx_type)
	{
	case FX_ROTOBACK:
		t=scn.timesc*0.33;
		glTranslatef(cos(t/2)*16, sin(t)*16, 0);
		glRotatef(t*53.3f+30,0.0,0.0,1.0);
		s=(2+sin(t/8))/2;
		glScalef(s, s, 1.0);
		break;
	case FX_WOBBLE:
		v=scn.fade*2;
		v=(v>1)?1:v*v;
		t=scn.timesc*0.33;
		glScalef(1+0.025*sin(t*16), 1+0.025*sin(t*17),1.0);
		break;
	case FX_TVFADE:
		v=scn.fade*2;
		v=(v>1)?1:v*v;
		glScalef(1,1-v*v*v,1.0);
		if (v>0.95) color.Alpha=0;
		break;
	case FX_JUMPINLEFT:
		v=scn.fade*2;
		v=(v>1)?1:v*v;
		glTranslatef(v*v*80, 0, 0);
		break;
	case FX_WIGGLE:
		v=sin(trigval()*1.57);
		glTranslatef(sin(6.28*v)*fx_xmagnitude*parent->lv_xscale*10,cos(6.28*v)*fx_ymagnitude*parent->lv_yscale*10,0);
		break;
	case FX_SCALE:
		v=beatprofile(trigval());
		glScalef(1+v*fx_xmagnitude,1+v*fx_ymagnitude,0);
		break;
	case FX_ROTATE:
		v=trigprofiled();
		glRotatef(v*fx_angle,0.0,0.0,1.0);
		break;
	case FX_LIGHT:
		if (guitarScene.timenow<=fx_light_number*44100)
		{
			color=C_TRANSPARENT;
			return;
		}
		v=trigval()*0.9;
		if (v<0) v=0;
		else if (v<0.2) v=v*8;
		else if (v<1) v=2-v*2;
		else v=0;
		if (v<0.01) return;
		mcolor efcolor(1,1,0.8,0.8);
		color.weight(color,efcolor,v*fx_intensity);
		break;
	}
}



