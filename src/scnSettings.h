#ifndef __HDR_scnSettings_h
#define __HDR_scnSettings_h


#include "includes.h"
#include "globals.h"
#include "sprites.h"
#include "scene.h"
#include "scnMenu.h"

class tScnSettings:public tScnMenuBase
{
public:
	tScnSettings() { scenes.add(this,SCN_SETTINGS); }
	GLfloat fadespeed() { return 100; }
	void init();
	void render();
	void itemClicked(int i);
};


#endif
