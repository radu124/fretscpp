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

#include "sgStats.h"

#include "scnGuitar.h"
#include "font.h"
#include "player.h"
#include "globals.h"
#include "audio.h"
#include "configuration.h"
#include "texManager.h"
#include "audiosong.h"

void tSceneGuitar::statPosition()
{
	GLfloat xp,yp,xsc,gsc,xlim;
	int sp=neckpos[activeneckpos].statpos;

	gsc=neckpos[activeneckpos].scale;
	xsc=gsc*neckpos[activeneckpos].stretch;
	if (xsc<gsc) gsc=xsc;
	xp=30*(-1+2*sp)*gsc;
	xp+=neckpos[activeneckpos].xdisp*6;
	yp=-neckpos[activeneckpos].ydisp*6;
	yp-=gsc*10;
	xlim=40-5*gsc;
	if (xp<-xlim) xp=-xlim;
	if (xp>xlim) xp=xlim;

	multvalx=xp;
	multvaly=yp;
	staty=yp+13*gsc;
	statfontsize=3*gsc;
	multvalscale=gsc;
	if (sp) {
		statx=xp+5*gsc;
		statalign=TFont::ALIGN_RIGHT;
	} else {
		statx=xp-5*gsc;
		statalign=0;
	}
}

void tSceneGuitar::renderMultiplierVal()
{
	tPlayer &pp=player[cplayer];
	int mult=pp.streak/10*2;
	int mr=pp.streak%10;
	if (pp.streak>=40) { mult=6; mr=10; }
	glPushMatrix();
	statPosition();
	glTranslatef(multvalx,multvaly,0);
	glScalef(multvalscale*5,-multvalscale*10,0);

	//deffont.displayString(".",0,1,1,0);
	texBind(sp_guitar[mult]);
	glColor4f(1,1,1,1);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1, -1, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f( 1, -1, 0);
	glTexCoord2f(0.0, 1.0-mr/10.0); glVertex3f(-1, mr/5.0-1, 0);
	glTexCoord2f(1.0, 1.0-mr/10.0); glVertex3f( 1, mr/5.0-1, 0);
	glEnd();
	texBind(sp_guitar[mult+1]);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0, 1.0-mr/10.0); glVertex3f(-1, mr/5.0-1, 0);
	glTexCoord2f(1.0, 1.0-mr/10.0); glVertex3f( 1, mr/5.0-1, 0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1, 1, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f( 1, 1, 0);
	glEnd();
	texUnbind();
	glPopMatrix();
}

void tSceneGuitar::renderStats()
{
	tPlayer &pp=player[cplayer];
	char a[256];
	glLoadIdentity();
	glColor4f(1.0,1.0,1.0,1.0);
	sprintf(a,"streak: %d",pp.streak);
	deffont.displayString(a,statx,staty,statfontsize,statalign);
	sprintf(a,"score: %d",pp.score);
	deffont.displayString(a,statx,staty+statfontsize,statfontsize,statalign);
}

void tSceneGuitar::globalStats()
{
	char a[256];
	int v1,v2;
	glLoadIdentity();
	glColor4f(1.0,1.0,1.0,1.0);
	switch (statsmode)
	{
	case 0:
		v1=playing.playpos/44100;
		v2=crtSong.lastevent/44100;
		sprintf(a,"%d.%02d/%d.%02d",v1/60,v1%60,v2/60,v2%60);
		deffont.displayString(a,-38,-28,2,0);
		break;
	case 1:
		sprintf(a,"playing: %d",songfiles[0].playpos);
		deffont.displayString(a,-38,-28,2,0);
		sprintf(a,"decode: %d-%d-%d",songfiles[0].decodepos,songfiles[1].decodepos,songfiles[2].decodepos);
		deffont.displayString(a,-38,-26,2,0);
		sprintf(a,"stop: %d-%d-%d",songfiles[0].stopsat,songfiles[1].stopsat,songfiles[2].stopsat);
		deffont.displayString(a,-38,-24,2,0);
		break;
	case 2:
		sprintf(a,"Whammy: %d %d",player[0].whammy,player[1].whammy);
		deffont.displayString(a,-38,-28,2,0);
		sprintf(a,"crtnote: %d %d  farnote: %d %d",
			player[0].crtnote, player[1].crtnote,
			player[0].farrnote, player[1].farrnote
			);
		deffont.displayString(a,-38,-26,2,0);
		sprintf(a,"shouldstart: %d time %d",shouldstillstart,timenow);
		deffont.displayString(a,-38,-24,2,0);

		break;
	}
}



