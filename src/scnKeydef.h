#ifndef __HDR_scnKeydef_h
#define __HDR_scnKeydef_h


#include "includes.h"
#include "scene.h"
#include "scnTable.h"
#include "midiparser.h"

class tScnKeydef:public tScnTable
{
public:
	tScnKeydef() { scenes.add(this,SCN_KEYDEF); }
	void enter();
	void handleevent(SDL_Event &a);
	void itemClicked(int i, int j);
};


#endif
