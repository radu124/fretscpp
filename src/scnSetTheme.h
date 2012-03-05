#ifndef __HDR_scnSetTheme_h
#define __HDR_scnSetTheme_h


#include "scnSetBase.h"

class tScnSetTheme:public tScnSetBase
{
public:
	tScnSetTheme() { scenes.add(this,SCN_SETTHEME); }
	GLfloat fadespeed() { return 100; }
	void init();
	void itemAdjusted(int i);
	void render();
};

extern tScnSetTheme scnSetTheme;
void theme_init();


#endif
