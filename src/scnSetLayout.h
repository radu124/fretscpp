#ifndef __HDR_scnSetLayout_h
#define __HDR_scnSetLayout_h


#include "scnSetBase.h"

class tScnSetLayout:public tScnSetBase
{
public:
	int np;
	int crtplayer;
	tSIintrange *sicrtpl;
	tScnSetLayout() { scenes.add(this,SCN_SETLAYOUT); }
	void init();
	void setTarget();
	void render();
	void itemAdjusted(int i);
	vector<string> statpositions;
};


#endif
