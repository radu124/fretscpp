#ifndef __HDR_scnSetGameplay_h
#define __HDR_scnSetGameplay_h


#include "scnSetBase.h"

class tScnSetGameplay:public tScnSetBase
{
public:
	tScnSetGameplay() { scenes.add(this,SCN_SETGAMEPLAY); }
	void init();
	vector<string> tappablemodes;
};


#endif
