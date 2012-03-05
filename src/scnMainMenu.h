#ifndef __HDR_scnMainMenu_h
#define __HDR_scnMainMenu_h


#include "includes.h"
#include "globals.h"
#include "sprites.h"
#include "scene.h"
#include "scnMenu.h"

class tScnMenuMain:public tScnMenuBase
{
public:
	tScnMenuMain() { scenes.add(this,SCN_MAINMENU); }
	GLfloat fadespeed() { return 1; }
	void init();
	void render();
	void itemClicked(int i);
};

extern tScnMenuMain MainMenu;

#endif
