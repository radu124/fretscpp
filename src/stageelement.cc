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
#define SLPD SLPD_INIT
	SLINI_LIST
#undef SLPD
}

void tStageElem::read(char *line)
{
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
	glPushMatrix();
	if (lv_src_blending) glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	color=lv_color;
	for (i=0; i<fx.size(); i++) fx[i]->apply(this);
	color.set();
	texDraw(texid);
	if (depth<15)
		for (i=0; i<children.size(); i++)
			children[i]->render(depth+1);
	if (lv_src_blending) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPopMatrix();
}
