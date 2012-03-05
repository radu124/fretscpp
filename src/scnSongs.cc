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

#include "scnSongs.h"

#include "songsdb.h"
#include "sprites.h"
#include "audio.h"
#include "score.h"
#include "seScore.h"
#include "globals.h"
#include "stage.h"
#include "texManager.h"
#include "font.h"
#include "audiosong.h"

tScnSongs scnSongs;

tScnSongs::tScnSongs()
{
	scenes.add(this,SCN_SONGS);
	previewsong=-1;
	previewlock=-1;
}

void tScnSongs::enter()
{
	SDL_EnableKeyRepeat(133,33);
	if (songsdb.size()==0) updatesongdb();
	selectsong(selected);
	sceneBase::enter();
}

void tScnSongs::render()
{
	GLfloat x,y;
	st_slist->render();
	scale=3;
	y=-24;
	x=-37;
	if (songsdb.size()==0) updatesongdb();

	int i;

	for (i=viewOffset; i<viewOffset+16 && i<songsdb.size(); i++)
	{
		glLoadIdentity();
		glColor3f(0.0f,0.0f,0.0f);
		GLfloat dy=0;
		string item=songsdb[i].name;
		if (selected==i && scn.fade==0)
			dy=sin(scn.time*8)*scale*0.1;
		//deffont.displayString(item.c_str(),x-scale*0.05,y+scale*0.05+dy,scale);

		if (selected==i && scn.fade==0)
			glColor3f(1.0f,0.7f,0.0f);
		else if (previewlock==i)
		{
			glColor3f(0.9f,0.1f,0.0f);
		}
		else {
			glColor3f(0.8f,0.3f,0.0f);
		}
		if (previewlock==i)
			item+=" (preview)";
		deffont.displayString(item.c_str(),x           ,y+dy           ,scale);
		//deffont.displayString("====",0           ,i*32           ,32);
		y=y+scale;
	}
	if (previewlock>=0)
	{
		if (previewlock!=previewsong)
		{
			previewsong=previewlock;
			string sp=songsdb[previewlock].dir+"/";
			song_start(sp.c_str());
		}
	}
	else if (previewsong!=selected && scn.time>selecttime+0.5)
	{
		previewsong=selected;
		string sp=songsdb[selected].dir+"/";
		song_start(sp.c_str());
	}
	renderStats();
	st_slist->renderForeground();
}

void tScnSongs::renderStats()
{
	if (!midiok) return;
	int i;
	GLfloat h=35;
	GLfloat wi=25;
	GLfloat posx=40-wi-1;
	GLfloat posy=-29;
	if (selected-viewOffset<8) posy=29-h;
	texUnbind();
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.0f,0.0f,0.0f,0.30f);
	glVertex3f(posx   ,posy   ,0);
	glVertex3f(posx+wi,posy   ,0);
	glVertex3f(posx+wi,posy+h ,0);
	glVertex3f(posx   ,posy+h ,0);
	glEnd();
	glPushMatrix();
	glTranslatef(posx+1,posy+1,0);
	glScalef(1.5,1.5,1);
	h/=1.5;
	posy=0;
	for (i=0; i<crtSong.trk_notes.size(); i++)
	{
		scoreStarsInstrument(i,posy);
	}
	i=0;
	posy+=1;
	// should add a nice scroll here
	while (posy<h-1)
	{
		if (i>=scores.history.size()) break;
		deffont.displayString(scores.history[i].c_str(),0,posy,1,0);
		posy+=1;
		i++;
	}
	glPopMatrix();
}

void tScnSongs::selectsong(int s)
{
	selected=s;
	if (selected>=songsdb.size())
	{
		midiok=0;
		return;
	}
	selectedsong=songsdb[selected].dir;
	midiok=crtSong.openfile((selectedsong+"/notes.mid").c_str());
	scores.setCurrent();
	selecttime=scn.time;
}

void tScnSongs::handleevent(SDL_Event &event)
{
	int oldsel=selected;
	int actkey;
	switch (event.type)
	{
	case SDL_KEYDOWN:
		actkey=event.key.keysym.sym;
		// may need to process this one to intercept other keys
		switch (actkey)
		{
		case SDLK_ESCAPE:
			scn.nextscene = 1;
			break;
		case SDLK_DOWN:
			if (selected<songsdb.size()-1) selected++;
			if (selected>viewOffset+15) viewOffset++;
			break;
		case SDLK_UP:
			if (selected>0) selected--;
			if (selected<viewOffset) viewOffset--;
			break;
		case SDLK_PAGEDOWN:
			selected=selected+16;
			if (selected>=songsdb.size()) selected=songsdb.size()-1;
			if (selected>viewOffset+15) viewOffset=selected-15;
			break;
		case SDLK_PAGEUP:
			selected=selected-16;
			if (selected<0) selected=0;
			if (selected<viewOffset) viewOffset=selected;
			break;
		case SDLK_HOME:
			selected=0;
			viewOffset=0;
			break;
		case SDLK_END:
			selected=songsdb.size()-1;
			if (selected>viewOffset+15) viewOffset=selected-15;
			break;
		case SDLK_SPACE:
			if (previewlock!=selected) previewlock=selected;
			else previewlock=-1;
			break;
		case SDLK_RETURN:
			if (!midiok) break;
			selectedsong=songsdb[selected].dir;
			fadetoscene(SCN_SETSONGOPT);
			break;
		}
		break;
	}
	if (selected!=oldsel) selectsong(selected);
}



