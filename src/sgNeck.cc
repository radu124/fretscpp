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

#include "sgNeck.h"

#include "scnGuitar.h"
#include "configuration.h"
#include "sprites.h"
#include "texManager.h"

void tSceneGuitar::renderNeck()
{
	GLfloat s0,s1,s3;
	s0=timenow*0.00001;
	s0=s0-(int) s0;
	s0=1-s0;
	s1=s0+0.2;
	s3=s0+0.4+0.2*boardlen;
	//glScalef(5.0f,5.0f,5.0f);
	texBind(sp_neck);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glBegin(GL_TRIANGLE_STRIP);

	glColor4f(1.0f,1.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,s0);
	glVertex3f(-2.3,2.0f,0);
	glTexCoord2f(1.0f,s0);
	glVertex3f(2.3,2.0f,0);

	glColor4f(1.0f,1.0f,1.0f,1.0f-themeNecktransparency*0.01);
	glTexCoord2f(0.0f,s1);
	glVertex3f(-2.3,1.0f,0);
	glTexCoord2f(1.0f,s1);
	glVertex3f(2.3,1.0f,0);

	glTexCoord2f(0.0f,s3);
	glVertex3f(-2.3,-boardlen,0);
	glTexCoord2f(1.0f,s3);
	glVertex3f(2.3,-boardlen,0);

	glEnd();
}

void tSceneGuitar::renderTracks()
{
	int i;
	GLfloat c0=0.7,c1=0.6,c2=0.3;
	texBind(sp_thinline);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	double lw=0.03;
	for (i=-2; i<3; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColor4f(c0,c1,c2,0.0f);
		glTexCoord2f(0.0f,0);
		glVertex3f(i-lw,2.5f,0);
		glTexCoord2f(1.0f,0);
		glVertex3f(i+lw,2.5f,0);

		glColor4f(c0,c1,c2,1.0f);
		glTexCoord2f(0.0f,0.2);
		glVertex3f(i-lw,2,0);
		glTexCoord2f(1.0f,0.2);
		glVertex3f(i+lw,2,0);

		glTexCoord2f(0.0f,1);
		glVertex3f(i-lw,-boardlen,0);
		glTexCoord2f(1.0f,1);
		glVertex3f(i+lw,-boardlen,0);
		glEnd();
	}
	int tbar=((int)timenow/44100)*44100;
	while (1)
	{
		GLfloat tpos=notePos(tbar);
		tbar+=44100;
		if (tpos>necktop) break;
		if (tpos<-boardlen) continue;
		glBegin(GL_TRIANGLE_STRIP);
		glColor4f(c0,c1,c2,1.0f);
		glTexCoord2f(0.0f,0.2); glVertex3f(-2.5,tpos+lw,0);
		glTexCoord2f(1.0f,0.2); glVertex3f(-2.5,tpos-lw,0);

		glTexCoord2f(0.0f,1); glVertex3f(2.5,tpos+lw,0);
		glTexCoord2f(1.0f,1); glVertex3f(2.5,tpos-lw,0);
		glEnd();
	}
	texUnbind();
}




