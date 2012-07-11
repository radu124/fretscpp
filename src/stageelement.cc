/*******************************************************************
(C) 2011,2012 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

#include "stage.h"

#include "configuration.h"
#include "texManager.h"

MULTIRDR(SLBLENDTYPE,"default,one")

tStageElem::tStageElem()
{
	cropleft=0;
	cropright=0;
	cropbtm=0;
	croptop=0;
	cropmode=0;
	lv_showif=parseNumExpression("1");
#define SLPD SLPD_INIT
	SLINI_LIST
#undef SLPD
}

tStageElem::~tStageElem()
{
	delete lv_showif;
}

void tStageElem::read(char *line)
{
	if (tsimatch(line,"showif")) { delete lv_showif; lv_showif=parseNumExpression(line); return; }
#define SLPD SLPD_READ
	SLINI_LIST
#undef SLPD
}

void tStageBackground::render(int depth)
{
	if (video_globalcleardisabled)
	{
		glColor4f( lv_color.Red, lv_color.Green, lv_color.Blue, 1.0 );
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-scr_lrlim, -scr_tblim, 0);
		glVertex3f( scr_lrlim, -scr_tblim, 0);
		glVertex3f(-scr_lrlim,  scr_tblim, 0);
		glVertex3f( scr_lrlim,  scr_tblim, 0);
		glEnd();
	}
	else
	{
		// plain clear, this may be faster?
		glClearColor( lv_color.Red, lv_color.Green, lv_color.Blue, 0 );
		glClear( GL_COLOR_BUFFER_BIT );
	}
	return;
}

void tStageElem::render(int depth)
{
	int i;
	if (lv_showif->val()<0.5) return;
	glPushMatrix();
	if (lv_src_blending) glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	color=lv_color;
	for (i=0; i<fx.size(); i++) fx[i]->apply(this);
	// optimize with and without crop since crop is so wasteful
	if (texid>=0)
	{
		color.set();
		if (!cropmode)
		{
			texDraw(texid);
		}
		else
		{
			GLfloat txhe,txwi;
			texBindGetSize(texid,txhe,txwi);
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(txwi*cropleft     , txhe*(1-cropbtm)); glVertex3f(-1+2*cropleft , -1+2*cropbtm, 0);
			glTexCoord2f(txwi*(1-cropright), txhe*(1-cropbtm)); glVertex3f( 1-2*cropright, -1+2*cropbtm, 0);
			glTexCoord2f(txwi*cropleft     , txhe*croptop);     glVertex3f(-1+2*cropleft ,  1-2*croptop, 0);
			glTexCoord2f(txwi*(1-cropright), txhe*croptop);     glVertex3f( 1-2*cropright,  1-2*croptop, 0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D,0);
		}
	}
	if (lv_src_blending) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (depth<15)
		for (i=0; i<children.size(); i++)
			children[i]->render(depth+1);
	glPopMatrix();
}
