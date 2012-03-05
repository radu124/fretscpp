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

#include "scnGuitar.h"

#include "audio.h"
#include "player.h"
#include "keyboard.h"
#include "globals.h"
#include "sgFlames.h"
#include "sgNeck.h"
#include "sgNotes.h"
#include "sgKeys.h"
#include "sgStats.h"
#include "stage.h"
#include "texManager.h"
#include "audiosong.h"

tSceneGuitar guitarScene;
GLfloat keycolors[5][3]={
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f
	};

void tSceneGuitar::updatetime()
{
	//MESSAGE("fade: %f\n" &scn.fade);
	if (scn.fade>0.0001)
	{
		if (scn.fade<0.66) timenow=(int) (-133636*scn.fade);
		else (timenow=-88200-9000*sin((scn.fade-0.66)*9.51));
	} else {
		if (shouldstillstart && !menumode)
		{
			MESSAGE("Starting playback.\n");
			shouldstillstart=0;
			playing.pause=0;
		}
		timenow=playing.playpos;
	}

}

void tSceneGuitar::render()
{
//	fprintf(stderr,"P%d:S%d:E%d:Pl%d %d/%d-%d-%d--%d-%d-%d\n",playing.pause,playing.stalled,playing.songended,playing.playing,playing.playpos,
//		playing.stopsat[0],playing.stopsat[1],playing.stopsat[2],playing.decodepos[0],playing.decodepos[1],playing.decodepos[2]);

	if (playing.songended)
	{
		fadetoscene(SCN_SCORE);
		MESSAGE("SONGENDED");
	}
	glClearColor( 0.3f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	stagePlay->render();
	updatetime();
	for (cplayer=0; cplayer<numplayers; cplayer++)
	{
		cinstrument=player[cplayer].instrument;
		cdifficulty=player[cplayer].difficulty;
		cnotes=&crtSong.trk_notes[cinstrument];
		scene_setNeck(cplayer,numplayers);
		noteRegion();
		player[cplayer].passtime();
		renderNeck();
		renderTracks();
		renderNoteLines();
		renderKeys();
		renderNotes();
		renderFlames();
		renderMultiplier();
	}
	scene_setOrtho();
	for (cplayer=0; cplayer<numplayers; cplayer++)
	{
		activeneckpos=cplayer+numplayers*(numplayers-1)/2;
		cinstrument=player[cplayer].instrument;
		cdifficulty=player[cplayer].difficulty;
		cnotes=&crtSong.trk_notes[cinstrument];
		renderMultiplierVal();
		renderStats();
	}
	globalStats();
	stagePlay->renderForeground();
	if (menumode)
	{
		glLoadIdentity();
		texUnbind();
		glBegin(GL_TRIANGLE_FAN);
		glColor4f(0.0f,0.0f,0.0f,0.30f);
		glVertex3f(-scr_lrlim,-scr_tblim,0);
		glVertex3f( scr_lrlim,-scr_tblim,0);
		glVertex3f( scr_lrlim, scr_tblim,0);
		glVertex3f(-scr_lrlim, scr_tblim,0);
		glEnd();
		glLoadIdentity();
		rendertext();
	}
}

void tSceneGuitar::enter()
{
	int i;
	tScnMenuBase::enter();
	MESSAGE("entered GUITAR\n");
	SDL_EnableKeyRepeat(0,0);
	for (i=0; i<numplayers; i++) player[i].init(i);
	menumode=0;
	statsmode=0;
	shouldstillstart=1;
	song_start((selectedsong+"/").c_str(),1);
	playing.pause=1;
}

void tSceneGuitar::init()
{
	if (initialized) return;
	INFO(SCNGUITAR,"Init guitar scene\n");
	additem("Return to Song");
	additem("Restart Song");
	additem("Change Song");
	additem("End Song");
	additem("Settings");
	additem("Quit to Main Menu");
	menumode=1;
	posx=-24;
	posy=-6;
	scale=3;
	timelasthit=timelastmiss=-1000000;
	tScnMenuBase::init();
	viewOffset=0;
	viewSize=6;
}

void tSceneGuitar::handleevent(SDL_Event &a)
{
	int ks=a.key.keysym.sym;
	if (a.type==SDL_KEYDOWN && ks==SDLK_ESCAPE && !menumode)
	{
		menumode=1;
		playing.pause=1;
		return;
	}

	if (a.type==SDL_KEYDOWN && ks=='`') statsmode=(statsmode+1)%4;

	if (menumode) tScnMenuBase::handleevent(a);
	else {
		if (a.type==SDL_KEYDOWN) guitarkey(a.type,a.key.keysym.sym);
		if (a.type==SDL_KEYUP) guitarkey(a.type,a.key.keysym.sym);
		if (a.type==SDL_JOYAXISMOTION)
		{
			int axisid=3000000+a.jaxis.which*256+a.jaxis.axis;
			//MESSAGE("WHAMMYAXIS %d %d\n" &axisid &a.jaxis.value);
			guitarkey(a.type,axisid,a.jaxis.value);
		}
	}
}

void tSceneGuitar::itemClicked(int i)
{
	switch (i)
	{
	case -1:
	case 0: // return
		menumode=0;
		playing.pause=0;
		return;
	case 1: // restart
		fadetoscene(SCN_PLAYING);
		return;
	case 2: // change song
		fadetoscene(SCN_SONGS);
		return;
	case 3: // change song
		fadetoscene(SCN_SCORE);
		return;
	case 4: // settings
		pushscene(SCN_SETTINGS);
		return;
	case 5: // main menu
		fadetoscene(SCN_MAINMENU);
		return;
	}
}







