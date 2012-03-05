#ifndef __HDR_scnMenu_h
#define __HDR_scnMenu_h


#include "includes.h"
#include "scene.h"

class tScnMenuBase: public sceneBase
{
public:
	vector<string> item;
	int selected;
	int viewOffset;
	int viewSize;
	GLfloat scale,posx,posy,faderot;
	void init();
	virtual void itemClicked(int i) {;}
	void enter();
	void additem(const char *v);
	void render();
	void rendertext();
	void renderback();
	void handleevent(SDL_Event &event);
};


#endif
