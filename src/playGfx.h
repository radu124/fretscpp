#ifndef __HDR_playGfx_h
#define __HDR_stagGfx_h

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
	int note[5];
	int notehl[5];
	int key[5];
};

#endif

