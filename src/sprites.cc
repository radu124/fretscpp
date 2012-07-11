/*******************************************************************
(C) 2010 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

#include "sprites.h"

#include "configuration.h"
#include "readpng.h"
#include "texManager.h"
#include "message.h"
#include "verbosity.h"
#include "vpxplayer.h"

int sp_neck;
int sp_ball;
int sp_flame;
int sp_glow;
int sp_hitg1, sp_hitg2;
int sp_hitf1, sp_hitf2;
int sp_noteline, sp_thinline;
int sp_guitar[8];
int sp_star1, sp_star2;
int sp_m2, sp_m3, sp_m4;

void sprite::bind()
{
	glBindTexture(GL_TEXTURE_2D,tex);
	boundframesago=0;
}

void sprite::load(const char *fname, int flags)
{
	string ffullname=datadir+"/"+fname;
	loadInternal(ffullname,flags);
}

void sprite::loadInternal(string ffullname, int flags)
{
	GLfloat cx=0.5f, cy=0.5f;
	int imgwidth=0,imgheight=0;
	int rwidth, rheight;
	int fnl=ffullname.length();
	isavi=(fnl>3
		&& ffullname[fnl-4]=='.'
		&& tolower(ffullname[fnl-3])=='a'
		&& tolower(ffullname[fnl-2])=='v'
		&& tolower(ffullname[fnl-1])=='i'
		);
	// get a texture id if we don't have one already
	if (isavi)
	{
		// load an AVI
		vp=new vpxplayer();
		isavi=vp->open(ffullname) && vp->decoder_init();
		if (!isavi)
		{
			INFO(SPRITES,"Failed loading AVI\n");
			delete vp;
		}
		else
		{
			//if (!tex)
				glGenTextures(1, &tex);
			vp->load_next_frame(tex,0,1);
			width=vp->width;
			height=vp->height;
			txwi=(float) vp->width/vp->glbufwidth;
			txhe=(float) vp->height/vp->glbufheight;
			INFO(SPRITES,"AVI texture: %d, size: %dx%d\n",tex,width,height);
		}
	}
	else
	{
		// load a PNG image
		loadImage(ffullname.c_str(),tex,imgwidth,imgheight,txwi,txhe);
		height=imgheight;
		width=imgwidth;
	}
	if (flags & 1) { height=10; width=10; }
	//tex= ilutGLLoadImage(ffullname.c_str());
	INFO(SPRITES,"Loading: %s, tex:%d size:%dx%d\n", ffullname, tex, width, height);
	if (!width)
	{
		MESSAGE("Could not load file: %s/%s\n", datadir, ffullname);
	}
	height/=10;
	width/=10;
}

void init_sprites()
{
	int i,j;
	sp_neck      = texLoad(themeNeckfilename.c_str());
	sp_ball      = texLoad("ball2.png");
	sp_hitg1     = texLoad("hitglow1.png");
	sp_hitg2     = texLoad("hitglow2.png");
	sp_hitf1     = texLoad("hitflames1.png");
	sp_hitf2     = texLoad("hitflames2.png");
	sp_glow      = texLoad("glow.png");
	sp_noteline  = texLoad("noteline.png");
	sp_thinline  = texLoad("thinline.png");
	sp_m2        = texLoad("2x.png");
	sp_m3        = texLoad("3x.png");
	sp_m4        = texLoad("4x.png");
	sp_star1     = texLoad("star1.png");
	sp_star2     = texLoad("star2.png");
}

void sprite::release()
{
	if (tex)
		glDeleteTextures(1,&tex);
	tex=0;
}


