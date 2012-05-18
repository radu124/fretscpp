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

// this file implements the flame rendering functions for the
// tSceneGuitar class declared in scnGuitar.h

#include "scnGuitar.h"
#include "includes.h"
#include "tappable.h"
#include "player.h"
#include "texManager.h"
#include "configuration.h"
#include "message.h"

void tSceneGuitar::renderMultiplier()
{
	tPlayer &pp=player[cplayer];
	if (timenow-pp.timemultiplier<40000)
	{
		GLfloat sv=(timenow-pp.timemultiplier)/8000.0;
		glPushMatrix();
		glTranslatef(0, 2, 0);
		glRotatef(60, 1, 0, 0);
		glTranslatef(0, 1, 0);
		glScalef(sv,sv,1);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		if (pp.streak>=30) texDraw(sp_m4);
		else if (pp.streak>=20) texDraw(sp_m3);
		else if (pp.streak>=10) texDraw(sp_m2);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPopMatrix();
	}
}

void tSceneGuitar::renderFlame(int col, GLfloat sz)
{
}

void tSceneGuitar::renderFlames()
{
	int i;
	tPlayer &pp=player[cplayer];

	for (i=0; i<5; i++)
	{
		if ((pp.hitactive && pp.lastkeys[i])
			|| (pp.lasthit[i]+10000>timenow))
		{
			rfFlame(i,1);
		}
	}

	//rfFlames2();
//	rfFlame(0,1);
//	origGlow(4);
}

void tSceneGuitar::rfFlame(int n, GLfloat sz)
{
	GLfloat x,y,v,f,ff,ms,flameSize,flameColor[4];
	GLfloat fcMod[3]={1.197,1.971,10.592};
	GLfloat fcMof[3]={1.197,1.784,12.222};
	v=0; // don't know
    f = 0.5; // = self.fretWeight[n];
    GLfloat c[3]={1,1,1}; //= self.fretColors[n];
    if (0) //f and (controls.getState(self.actions[0]) or controls.getState(self.actions[1])):
		f += 0.25;
	y = v + f / 6;
	x = n-2;
	f = 0.1; //self.fretActivity[n]

	if (f==0) return;

	ms = sin(scn.timesc*1000) * .25 + 1;
	ff = f+1.2;

	glBlendFunc(GL_ONE, GL_ONE);

	flameSize = 0.2;//self.flameSizes[self.scoreMultiplier - 1][n]
	flameColor[0] = 0.6;
	flameColor[1] = 0.5;
	flameColor[2] = 0.05;//self.flameColors[self.scoreMultiplier - 1][n]

	//if flameColor[0] == -2:
	//	flameColor = self.fretColors[n]

	glColor3f(flameColor[0] * fcMod[0], flameColor[1] * fcMod[1], flameColor[2] * fcMod[2]);
	glPushMatrix();
	glTranslatef(x, -boardline, 0);
	glRotatef(60, 1, 0, 0);
	glTranslatef(0, y, 0);
	glScalef((0.5 + .6 * ms * ff) * ff *flameSize, (1 + .6 * ms * ff)*ff*flameSize,1);
	glTranslatef(0, 0.5, 0);
	texDraw(sp_hitg1);
	glPopMatrix();

	ff += .3;

	//flameSize = self.flameSizes[self.scoreMultiplier - 1][n]
	//flameColor = self.flameColors[self.scoreMultiplier - 1][n]

	glColor3f(flameColor[0] * fcMof[0], flameColor[1] * fcMof[1], flameColor[2] * fcMof[2]);
	glPushMatrix();
	glTranslatef(x, -boardline, 0);
	glRotatef(60, 1, 0, 0);
	glTranslatef(0, y, 0);
	glScalef((.40 + .6 * ms * ff)*flameSize*ff, (1 + .6 * ms * ff) *flameSize*ff,1);
	glTranslatef(0, 0.5, 0);
	texDraw(sp_hitg2);
	glPopMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void tSceneGuitar::origGlow(int n)
{
	GLfloat s=0.0;
	GLfloat size[2];
	GLfloat x,y,t,sf,isf=0;
	GLfloat f=1;
	GLfloat ms;
	x=n-2;
	y=-boardline;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(f + scn.time * 100, 0, 0, 1);
	size[0] = .22 * (f + 1.5);
	size[1] = .22 * (f + 1.5);

	//if self.playedNotes:
		//t = cos(math.pi + (self.time - self.playedNotes[0][0]) * 0.01)
	//else:
		t = cos(scn.time * 10);

	sf=f*size[0];
	if (sf!=0) isf=1/sf;
	glScalef(sf,sf,0);

	while (s < .5)
	{
		ms = (1 - s) * f * t * .25 + .75;
		glColor3f(keycolors[n][0] * ms, keycolors[n][1] * ms, keycolors[n][2] * ms);
		texDraw(sp_glow);
		glTranslatef(0, ms * .1, 0);
		glScalef(.8, 1, .8);
		glRotatef(ms * 20, 0, 0, 1);
		s += 0.2;
	}
	glPopMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// to fix...
void tSceneGuitar::rfFlames2()
{
	GLfloat ms;
	int i,j;
	const int flameLimit = 10.0;
	const int flameLimitHalf = flameLimit/2;
    tPlayer &pp=player[cplayer];
	int evfc=0; //event.flameCount
    GLfloat sang=60;
    glBlendFunc(GL_ONE, GL_ONE);
    MESSAGE("\n");
    for (i=pp.crtnote-1; i>=0; i--)
	{
		MESSAGE(".");
		int nts=cnotes[0][i].timestamp;
		if (timenow-nts>100000) break;
		if (pp.history[i]!=2) continue;
		MESSAGE(">");
		for (j=0; j<5; j++)
		{
			char ch=cnotes[0][i].stat[cdifficulty*12+j];
			if (ch!='O' && ch!='B') continue;
			MESSAGE("%d", j);
			//int tev=player[cplayer].timelastgood[i];
			int mult;
			GLfloat mx,x,y,ff;
			GLfloat flameColor[3];
			GLfloat flameSize;
			ms = sin(timenow/44) * .25 + 1;

			x  = j-2; // (self.strings / 2 - event.number) * w
			ff = 1.25;
			y = -boardline; //-ff / 6; //v +

			flameSize = (10-9/(3+pp.streak))*((timenow-nts)/10000000.0);

			mult=pp.streak/10;
			if (mult>3) mult=3;
			flameColor[0] = keycolors[mult][0]; // ev.number
			flameColor[1] = keycolors[mult][1];
			flameColor[2] = keycolors[mult][2];

			ff += 1.5;
			if (i < flameLimitHalf)
			{
				glColor3f(flameColor[0], flameColor[1], flameColor[2]);
				glPushMatrix();
				glTranslatef(x, y + .20, 0);
				glRotatef(sang, 1, 0, 0);
				glScalef(.25 + .6 * ms * ff, evfc/6.0 + .6 * ms * ff, evfc / 6.0 + .6 * ms * ff);
				glScalef(flameSize *ff, flameSize *ff, 1);
				texDraw(sp_hitf2);
				glPopMatrix();

				glPushMatrix();
				glTranslatef(x - .005, y + .25 + .005, 0);
				glRotatef(sang, 1, 0, 0);
				glScalef(.30 + .6 * ms * ff, (evfc + 1) / 5.5 + .6 * ms * ff, (evfc + 1) / 5.5 + .6 * ms * ff);
				glScalef(flameSize *ff, flameSize *ff, 1);
				texDraw(sp_hitf2);
				glPopMatrix();

				glPushMatrix();
				glTranslatef(x+.005, y +.25 +.005, 0);
				glRotatef(sang, 1, 0, 0);
				glScalef(.35 + .6 * ms * ff, (evfc + 1) / 5.0 + .6 * ms * ff, (evfc + 1) / 5.0 + .6 * ms * ff);
				glScalef(flameSize *ff, flameSize *ff, 1);
				texDraw(sp_hitf2);
				glPopMatrix();

				glPushMatrix();
				glTranslatef(x, y +.25 +.005, 0);
				glRotatef(sang, 1, 0, 0);
				glScalef(.40 + .6 * ms * ff, (evfc + 1)/ 4.7 + .6 * ms * ff, (evfc + 1) / 4.7 + .6 * ms * ff);
				glScalef(flameSize *ff, flameSize *ff, 1);
				texDraw(sp_hitf2);
				glPopMatrix();
			}
			else
			{
				GLfloat flameColorMod0 = 0.1 * (flameLimit - evfc);
				GLfloat flameColorMod1 = 0.1 * (flameLimit - evfc);
				GLfloat flameColorMod2 = 0.1 * (flameLimit - evfc);

				glColor3f(flameColor[0] * flameColorMod0, flameColor[1] * flameColorMod1, flameColor[2] * flameColorMod2);
				glPushMatrix();
				glTranslatef(x, y + .35, 0);
				glRotatef(sang, 1, 0, 0);
				glScalef(.25 + .6 * ms * ff, evfc / 3.0 + .6 * ms * ff, evfc / 3.0 + .6 * ms * ff);
				glScalef(flameSize *ff, flameSize *ff, 1);
				texDraw(sp_hitf1);
				glPopMatrix();


				glPushMatrix();
				glTranslatef(x - .005, y + .40 + .005, 0);
				glRotatef(sang, 1, 0, 0);
				glScalef(.30 + .6 * ms * ff, (evfc + 1)/ 2.5 + .6 * ms * ff, (evfc + 1) / 2.5 + .6 * ms * ff);
				glScalef(flameSize *ff, flameSize *ff, 1);
				texDraw(sp_hitf1);
				glPopMatrix();


				glPushMatrix();
				glTranslatef(x + .005, y + .35 + .005, 0);
				glRotatef(sang, 1, 0, 0);
				glScalef(.35 + .6 * ms * ff, (evfc + 1) / 2.0 + .6 * ms * ff, (evfc + 1) / 2.0 + .6 * ms * ff);
				glScalef(flameSize *ff, flameSize *ff, 1);
				texDraw(sp_hitf1);
				glPopMatrix();


				glPushMatrix();
				glTranslatef(x+.005, y +.35 +.005, 0);
				glRotatef(sang, 1, 0, 0);
				glScalef(.40 + .6 * ms * ff, (evfc + 1) / 1.7 + .6 * ms * ff, (evfc + 1) / 1.7 + .6 * ms * ff);
				glScalef(flameSize *ff, flameSize *ff, 1);
				texDraw(sp_hitf1);
				glPopMatrix();
			}
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



