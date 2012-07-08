#ifndef __HDR_stage_h
#define __HDR_stage_h


#include "includes.h"
#include "globals.h"
#include "scene.h"
#include "sprites.h"
#include "stagefx.h"
#include "stageelement.h"

class Stage
{
public:
	/**
	 * List of elements
	 */
	vector<tStageElem*> elem;
	/**
	 * List of layers
	 */
	vector<tStageFx*> fx;
	/**
	 * load the stage description from a file, including all required
	 * textures
	 */
	virtual void load(string dir, string filename="stage.ini");
	/**
	 * deallocate everything
	 */
	void cleanup();
	/**
	 * render the non-foreground part of the stage,
	 * all the elements not marked foreground=1
	 */
	void render();
	/**
	 * Render the foreground of the stage
	 * for example what should come above menu text or
	 * guitar neck
	 *
	 * these are the elements marked with foreground=1
	 */
	void renderForeground();
	/**
	 * Return a pointer to the element with the given name,
	 * create the element if it does not exist
	 * @param name the Element name
	 * @return pointer to Element
	 */
	tStageElem *findElem(string name);
	/**
	 * Return a pointer to the FX with the given name,
	 * create the FX if it does not exist create an effect of the
	 * type typ
	 * @param name the FX name
	 * @return pointer to FX
	 */
	tStageFx *findFX(string name, string typ="");
	~Stage() { cleanup();}
};


#endif
