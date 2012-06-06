#ifndef __HDR_scene_h
#define __HDR_scene_h


#include "includes.h"

/**
 * Scenes currently identified by numeric ids
 * TODO: switch to strings and get rid of the enum
 */
enum ENUM_SCENES { SCN_NULL=0, SCN_MAINMENU=1, SCN_SONGS=2, SCN_SETSONGOPT=3, SCN_PLAYING=4,
	SCN_KEYDEF, SCN_SCORE, SCN_SETTINGS, SCN_SETGAMEPLAY, SCN_SETLAYOUT, SCN_SETVIDEO, SCN_SETTHEME};

class sceneBase
{
public:
	/**
	 * function called to display the contents of the scene
	 * usually called when the scene is current, but also on
	 * a prevscene during fade-out
	 */
	virtual void render()=0;
	/**
	 * event handler called when the scene is current
	 */
	virtual void handleevent(SDL_Event &a)=0;
	/**
	 * virtual function that returns the speed of scene fade-in
	 * @return duration of fade-in (in seconds)
	 */
	virtual GLfloat fadespeed() { return 0.6; }
	/**
	 * virtual function that returns the speed of scene fade-out
	 * defaults to speed of fade-in
	 * @return duration of fade-out (in seconds)
	 */
	virtual GLfloat fadeoutspeed() { return fadespeed(); }
	/**
	 * virtual function called once, before the scene is used
	 */
	virtual void init() { initialized=1; }
	/**
	 * virtual function called when a scene becomes current
	 */
	virtual void enter() { if (!initialized) { init(); initialized=1; } }
	/**
	 * virtual function that is called when a scene ceases to be current
	 */
	virtual void leave() {;}
	int initialized;
};

struct tscene
{
	GLfloat fade;
	GLfloat menufade;
	/**
	 * timenow() as we start rendering
	 */
	GLfloat time;
	/**
	 * the time when the fade-in ended
	 * (during fade-in equal to time)
	 */
	double timestartsc;
	/**
	 * time elapsed since the scene has faded in
	 * (during fade-in equal to 0)
	 */
	GLfloat timesc;
	/**
	 * time when the scene was first selected
	 */
	GLfloat timestartfade;
	/**
	 * The current scene
	 * this scene is displayed (if no prevscene exists)
	 * and receives input (even if prevscene exists)
	 */
	int current;
	/**
	 * scene that we are currently fading out of
	 * this scene is still displayed even though not current
	 */
	int prevscene;
	/**
	 * scene that will be entered into on the next render
	 * 0 if the current scene keeps displaying
	 */
	int nextscene;
};

extern tscene scn;

/**
 * renders the current scene
 * or the previous scene during fade-out
 */
void renderscene();
/**
 * call the event handler of the current scene
 */
void scenehandleevent(SDL_Event &ev);
/**
 * immediately switch to the given scene (skip fade-out)
 * @param i the scene to switch to
 */
void jumptoscene(int i);
/**
 * save current scene on the scene stack and switch to another scene
 * @param i the scene to switch to
 */
void pushscene(int i);
/**
 * switch to another scene (with fade-out)
 * @param i the scene to switch to
 */
void fadetoscene(int i);
/**
 * exit the current scene and return to the previous scene on the stack
 */
void popscene();

/**
 * some scenes can be entered from multiple other scenes
 * (settings scenes)
 * this stack memorizes where we should return to
 */
extern vector<int> scenestack;

/**
 * class that keeps track of the available scenes
 * FIXME: singleton attempt, make nicer
 */
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
	sceneBase * operator [] (int i);
};

extern tscenelist scenes;

/**
 * set ortho view for menus
 * TODO: move these in a separate file
 */
void scene_setOrtho();
/**
 * set neck view
 * @param i the neck number (for multiplayer)
 * @param np the number of players
 */
void scene_setNeck(int i=0, int np=1);

/**
 * constants for neck view
 */
extern GLfloat camerapos[3];
/**
 * constant for neck view
 */
extern GLfloat cameraangle;


#endif
