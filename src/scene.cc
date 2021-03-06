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

#include "scene.h"

#include "scnMainMenu.h"
#include "message.h"
#include "scnSongs.h"
#include "scnMenu.h"
#include "globals.h"
#include "scnSetSongOpt.h"
#include "scnGuitar.h"
#include "scnScore.h"
#include "configuration.h"
#include "scnSettings.h"
#include "scnSetLayout.h"
#include "scnSetVideo.h"
#include "scnSetGameplay.h"
#include "scnSetTheme.h"
#include "keyboard.h"
#include "texManager.h"

GLfloat camerapos[3]={0,-4,3};
GLfloat cameraangle=30;

vector<int> scenestack;

tscene scn;
tscenelist scenes;

void scene_setOrtho()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	GLfloat lrlim=40; // left-right limit
	GLfloat tblim=30; // top-bottom limit
	// we change from the initial implementation
	// to a uniform resolution-independent coordinate system
	// centered in the center of the screen, xsize 80, ysize 60
	if (!conf_stretch)
	{
		// if the window aspect ratio is not 4/3, we add borders around
		// it to make sure we have a 4/3 cetral part that will fit the menus
		if (scr_width*30>scr_height*40) lrlim=(GLfloat) tblim *scr_width/scr_height;
		if (scr_width*30<scr_height*40) tblim=(GLfloat) lrlim *scr_height/scr_width;
	}
	// to avoid flipping everything vertically we keep the vertical axis
	// inverted
	glOrtho(-lrlim, lrlim, tblim, -tblim, -1.0f, 1.0f);
	scr_lrlim=lrlim;
	scr_tblim=tblim;
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

GLfloat sskewmatrix[16]={
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1};

void scene_setNeck(int i, int np)
{
	int active=i+np*(np-1)/2;
	assume(active>=0 && active<neckpos.size(),"trying to set wrong neckpos");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, 1.0f * scr_width / scr_height, 1.0f, 100.0f);
	gluLookAt(
		camerapos[0], camerapos[1], camerapos[2],
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f);
	//glLoadIdentity();

	//float fvViewMatrix[ 16 ];
	//glGetFloatv( GL_MODELVIEW_MATRIX, fvViewMatrix );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glTranslatef(neckpos[active].xdisp,neckpos[active].ydisp/2,neckpos[active].ydisp); //-1.5+3*i,0,0);
	glScalef(neckpos[active].scale*neckpos[active].stretch,neckpos[active].scale,1);
	sskewmatrix[4]=neckpos[active].xdisp*0.16+neckpos[active].skew; // Y adds to X
	sskewmatrix[6]=neckpos[active].ydisp*0.08; // Y adds to Z
	sskewmatrix[8]=-neckpos[active].xdisp*0.1;
	//sskewmatrix[4]=-0.4+i*0.8;
	//sskewmatrix[12]=-neckpos[active].xdisp;
	//sskewmatrix[8]=0.1-0.2*i;
	// ok, there are more ways of doing it, i like it like this
	//sskewmatrix[8]=0.8-1.6*i;
	glMultMatrixf(sskewmatrix);

}

void pushscene(int i)
{
	scenestack.push_back(scn.current);
	fadetoscene(i);
}

void popscene()
{
	int nxt;
	if (!scenestack.size()) nxt=0;
	else {
		nxt=scenestack.back();
		scenestack.pop_back();
	}
	//fadetoscene(nxt);
	scn.current=nxt;
}

void fadetoscene(int i)
{
	scn.nextscene=i;
}

void jumptoscene(int i)
{
	scn.nextscene=i;
	scn.prevscene=0;
}

sceneBase * tscenelist::operator [] (int i)
{
	assume(i>=0 && i<get().size(),"bad scene %d", i);
	return get()[i];
}

GLfloat fadespeed(int scene)
{
	if (!scenes[scene])
		return 0.6;
	return scenes[scene]->fadespeed();
}

GLfloat fadeoutspeed(int scene)
{
	if (!scenes[scene])
		return 0.6;
	return scenes[scene]->fadeoutspeed();
}

void scenehandleevent(SDL_Event &event)
{
	if (!scenes[scn.current]) return;
	int mkflags=0;
	if (scn.current==SCN_SCORE) mkflags=1;
	if (scn.current!=SCN_PLAYING && guitarScene.menumode==0) menukey(event,mkflags);
	scenes[scn.current]->handleevent(event);
}

void renderscene()
{
	// ensure no leftovers
	texUnbind();
	glLoadIdentity();

	scn.time=gettimenow();
	int crtdispscene=scn.current;
	if (scn.nextscene)
	{
		scenes[scn.current]->leave();
		scn.prevscene=scn.current;
		scn.current=scn.nextscene;
		scn.nextscene=0;
		scn.timestartfade=scn.time;
		scn.fade=1;
		scenes[scn.current]->enter();
	}
	if (scn.prevscene)
	{
		GLfloat fs=fadeoutspeed(scn.prevscene);
		scn.fade=(scn.time-scn.timestartfade)*fs;
		if (scn.fade>1 || fs>100)
		{
			scn.prevscene=0;
			scn.timestartfade=scn.time;
			scn.fade=1;
		}
		else crtdispscene=scn.prevscene;
	} else {
		scn.fade=1-(scn.time-scn.timestartfade)*fadespeed(scn.current);
		if (scn.fade<0) scn.fade=0;
		else scn.timestartsc=scn.time;
	}
	scn.timesc=scn.time-scn.timestartsc;

	if (!scenes[crtdispscene])
	{
		// bootstrap
		scn.current=SCN_MAINMENU;
		scn.timestartfade=scn.time;
	}
	else scenes[crtdispscene]->render();
}



