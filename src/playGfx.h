#ifndef __HDR_playGfx_h
#define __HDR_playGfx_h

#include "stage.h"

/**
 * Playgfx is a container for the gameplay elements
 * it uses Stage for settings loading, but it offers some shortcuts
 * to its contents
 */
class playGfx:public Stage
{
public:
	/**
	 * calls the normal load, but afterwards uploads the texids
	 * of the notes and other elements in arrays where they can be
	 * easily accessed
	 */
	void load(string dir, string filename="stage.ini");
	/**
	 * find with fallback
	 */
	tStageElem *ffallback(const char *preferred, const char *fallback, int num=0);
	tStageElem *note[5];
	tStageElem *notehl[5];
	tStageElem *key[5];
	tStageElem *keypressed[5];
	tStageElem *keystrummed[5];
	tStageElem *multiplierbar;
};

#endif

