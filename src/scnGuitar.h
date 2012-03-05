#ifndef __HDR_scnGuitar_h
#define __HDR_scnGuitar_h


#include "scene.h"
#include "scnMenu.h"
#include "midiparser.h"

class tSceneGuitar:public tScnMenuBase
{
public:
	GLfloat multvalx,multvaly,multvalscale;
	GLfloat statx,staty,statfontsize;
	int statalign;
	tSceneGuitar() { scenes.add(this,SCN_PLAYING); };
	GLfloat fadespeed() { return 0.33; }
	GLfloat fadeoutspeed() { return 1000; }
	int activeneckpos;
	int timenow;
	int timelasthit;
	int timelastmiss;
	int cplayer;
	int cinstrument;
	int cdifficulty;
	int statsmode;
	int shouldstillstart;
	vector<notestatus> *cnotes;
	void enter();
	void init();
	void render();
	void handleevent(SDL_Event &a);
	void renderNeck();
	void renderTracks();
	void renderBars();
	void renderKey(int num, int position);
	void renderKeys();
	int renderNote(int col, int ts, int flags=0);
	void renderNotes();
	void renderNoteLines();
	void renderNoteLine(int col, GLfloat from, GLfloat to, int flags);
	void renderFrets();
	void renderStats();
	void renderFlame(int col, GLfloat sz);
	void renderFlames();
	void renderMultiplier();
	void renderMultiplierVal();
	void noteRegion();
	void origGlow(int n);
	void rfFlame(int n, GLfloat sz);
	void rfFlames2();
	int menumode;
	void itemClicked(int i);
	GLfloat notePos(int timestamp);
	void updatetime();
	void statPosition();
	void globalStats();
};

extern tSceneGuitar guitarScene;
extern GLfloat keycolors[5][3];


#endif
