#ifndef __HDR_scnSetSongOpt_h
#define __HDR_scnSetSongOpt_h


#include "includes.h"
#include "scene.h"
#include "scnTable.h"
#include "midiparser.h"
#include "songini.h"

class tScnSetSongOpt:public tScnTable
{
public:
	tScnSetSongOpt() { scenes.add(this,SCN_SETSONGOPT); }
	void enter();
	void enableDisable(int i);
	int getInstrument(int i);
	int getDifficulty(int i);
	void selInstrument(int i, int j=-1);
	void selDifficulty(int i, int j=-1);
	void itemClickedSep(int i, int k, int j, int &o);
	void setKeySels(int i);
	int lastP2controller;

	/**
	 * If selected players are not the first ones reorder
	 * the list so they end up first
	 *
	 * set numPlayers to the appropriate value
	 */
	void regroupPlayers();
};

extern tScnSetSongOpt SongOpt;

#endif
