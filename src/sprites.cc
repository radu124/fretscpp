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

int sp_neck;
int sp_ball;
int sp_flame;
int sp_glow;
int sp_key[5];
int sp_note[5];
int sp_hitg1, sp_hitg2;
int sp_hitf1, sp_hitf2;
int sp_noteline, sp_thinline;
int sp_guitar[8];
int sp_star1, sp_star2;
int sp_m2, sp_m3, sp_m4;

void sprite::bind()
{
	glBindTexture(GL_TEXTURE_2D,tex);
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
	loadImage(ffullname.c_str(),tex,imgwidth,imgheight,txwi,txhe);
	height=imgheight;
	width=imgwidth;
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
	sp_guitar[0] = texLoad("guitar1ok.png");
	sp_guitar[1] = texLoad("guitar1gs.png");
	sp_guitar[2] = texLoad("guitar2ok.png");
	sp_guitar[3] = texLoad("guitar2gs.png");
	sp_guitar[4] = texLoad("guitar3ok.png");
	sp_guitar[5] = texLoad("guitar3gs.png");
	sp_guitar[6] = texLoad("guitar4ok.png");
	sp_guitar[7] = texLoad("guitar4gs.png");
	sp_star1     = texLoad("star1.png");
	sp_star2     = texLoad("star2.png");
	for (i=0; i<5; i++)
	{
		char a[32];
		sprintf(a,"key%d.png",i+1);
		sp_key[i]  = texLoad(a);
		sprintf(a,"note0%d.png",i+1);
		sp_note[i] = texLoad(a);
	}
}


