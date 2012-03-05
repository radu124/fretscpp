#ifndef __HDR_scnSongs_h
#define __HDR_scnSongs_h


#include "includes.h"
#include "verbosity.h"
#include "scene.h"

class tScnSongs:public sceneBase
{
public:
	tScnSongs();
	void enter();
	void render();
	void handleevent(SDL_Event &a);
	GLfloat fadespeed() { return 100; }
	GLfloat scale;
	int previewsong;
	int selected;
	int previewlock;
	int midiok;
	GLfloat selecttime;
	int viewOffset;
	void renderStats();
	void selectsong(int s);
};


#endif
