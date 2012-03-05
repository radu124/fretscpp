#ifndef __HDR_scnScore_h
#define __HDR_scnScore_h


#include "includes.h"
#include "scene.h"
#include "scnTable.h"
#include "midiparser.h"

class tScnScore:public sceneBase
{
public:
	tScnScore() { scenes.add(this,SCN_SCORE); }
	GLfloat fadespeed() { return 1; }
	GLfloat fadeoutspeed() { return 1000; }
	int editing;
	void enter();
	void render();
	void writescores();
	void handleevent(SDL_Event &a);
};


#endif
