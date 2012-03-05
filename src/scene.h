#ifndef __HDR_scene_h
#define __HDR_scene_h


#include "includes.h"

enum ENUM_SCENES { SCN_NULL=0, SCN_MAINMENU=1, SCN_SONGS=2, SCN_SETSONGOPT=3, SCN_PLAYING=4,
	SCN_KEYDEF, SCN_SCORE, SCN_SETTINGS, SCN_SETGAMEPLAY, SCN_SETLAYOUT, SCN_SETVIDEO, SCN_SETTHEME};

class sceneBase
{
public:
	virtual void render()=0;
	virtual void handleevent(SDL_Event &a)=0;
	virtual GLfloat fadespeed() { return 0.6; }
	virtual GLfloat fadeoutspeed() { return fadespeed(); }
	virtual void init() { initialized=1; }
	virtual void enter() { if (!initialized) { init(); initialized=1; } }
	virtual void leave() {;}
	int initialized;
};

struct tscene
{
	GLfloat fade;
	GLfloat menufade;
	GLfloat time;
	double timestartsc;
	GLfloat timesc;
	GLfloat timestartfade;
	int current;
	int prevscene;
	int nextscene;
};

extern tscene scn;

void renderscene();
void scenehandleevent(SDL_Event &ev);
void jumptoscene(int i);
void pushscene(int i);
void fadetoscene(int i);
void popscene();

extern GLfloat camerapos[3];
extern GLfloat cameraangle;

extern vector<int> scenestack;

class tscenelist
{
public:
	static vector<sceneBase*> & get()
	{
		static vector<sceneBase*> res;
		return res;
	}
	void add(sceneBase *s, ENUM_SCENES idx)
	{
		if (idx>=get().size()) get().resize(idx+1);
		get()[idx]=s;
	}
//	int add(sceneBase *s)
//	{
//		get().push_back(s);
//		return get().size()-1;
//	}
	sceneBase * operator [] (int i);
};

void scene_setOrtho();
void scene_setNeck(int i=0, int np=1);

extern tscenelist scenes;


#endif
