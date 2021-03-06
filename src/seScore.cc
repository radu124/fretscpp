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

#include "seScore.h"

#include "sprites.h"
#include "score.h"
#include "texManager.h"
#include "font.h"
#include "globals.h"
#include "configuration.h"

void showStars(int promille, GLfloat dx, GLfloat dy)
{
	GLfloat rcolor=1.0;
	int q=promille/200;
	int i;
	if (promille==1000) rcolor=0;
	for (i=0; i<q; i++)
	{
		glColor4f(rcolor,1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(dx+i-2,dy,0);
		glScalef(0.5,-0.5,0);
		texDraw(sp_star2);
		glPopMatrix();
	}
	for (i=q+1; i<5; i++)
	{
		glPushMatrix();
		glTranslatef(dx+i-2,dy,0);
		glScalef(0.5,-0.5,0);
		texDraw(sp_star1);
		glPopMatrix();
	}
	glColor4f(1.0,1.0,1.0,1.0);
	if (q==5) return;
	promille-=q*200+10;
	promille/=100;
	GLfloat dv=promille/2.0;
	dx+=q-2.5;
	texBind(sp_star2);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0, 0.0); glVertex3f(dx   , dy-0.5, 0);
	glTexCoord2f(dv , 0.0); glVertex3f(dx+dv, dy-0.5, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(dx   , dy+0.5, 0);
	glTexCoord2f(dv , 1.0); glVertex3f(dx+dv, dy+0.5, 0);
	glEnd();
	texBind(sp_star1);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(dv , 0.0); glVertex3f(dx+dv, dy-0.5, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(dx+1 , dy-0.5, 0);
	glTexCoord2f(dv , 1.0); glVertex3f(dx+dv, dy+0.5, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(dx+1 , dy+0.5, 0);
	glEnd();
	texUnbind();
}

void scoreStarsInstrument(int instrument, GLfloat &ypos)
{
	int i;
	glColor4f(1,0.8,0.3,1);
	ypos+=1;
	deffont.displayString(crtSong.trk_instrument[instrument].c_str(),0,ypos,1,0);
	ypos+=1.5;
	glColor4f(1,1,1,1);
	for (i=0; i<4; i++)
	{
		if (scores.best.size()<=i+instrument*4) continue;
		int v=scores.best[i+instrument*4];
		if (v==-2) continue;
		int pm=0;
		deffont.displayString(difficultynames[i],0,ypos,1,0);
		if (v>=0) {
			pm=scores.data[v].promille();
			showStars(pm,6,ypos-0.40);
			deffont.displayString(scores.data[v].playername,9,ypos,1,0);
		}
		ypos+=1;
	}
}



