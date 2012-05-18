#ifndef __HDR_player_h
#define __HDR_player_h

#include "includes.h"
#include "midiparser.h"

/**
 * Set the music volume based on player status
 */
void player_get_music_volume(int &gtar_le, int &gtar_rt, int &rhyt_le, int &rhyt_rt, int &wham_le, int &wham_rt);

class tPlayer
{
public:
	int lefty;
	int score;
	int streak;
	int lastkeys[6];
	int lasthit[6]; // time of last hit per key [5]=any
	int keys;
	int hit;
	int longeststreak;
	int crtnote;
	int nextnote;
	int donenote;
	int farrnote;
	int hittnote;
	int id;
	int crtnotehit;
	int multiplier;
	int instrument;
	int difficulty;
	int hitactive;
	int scorefrac;
	int notegood;
	int notetotal;
	int notexmiss;
	int notehopo;
	int lasttime; // last time we called passtime()
	int lastwasgood;
	int timemultiplier;
	// whammy accumulator
	int whammy;
	// whammy status on/off
	int whammyon;
	// when whammy is controlled by analog input, the last value of the
	// analog axis
	int whammyaxislast;
	// how much of the current note they player already
	// got credit for (timestamp)
	int holdaccounted;
	int enabled;
	string name;
	string lastInstr;
	int lastDiffi;
	int scorenomult;
	int scorehits;
	int scorehold;
	int scorewham;

	GLfloat neckvelocity;

	void handlehit();
	void passtime();
	void init(int iid);
	void presskey(int i);
	void releasekey(int i);
	void unhitactive(int ts);
	void holdscore(int ts);
	void addwhammy(int i);
	void whammyaxis(int v);
	int notematch(int i);

	/**
	 * initializes tracktap with an array of booleans representing
	 * tappable notes
	 */
	void tappable_init_track(int mode);

	vector<notestatus> *cnotes;
	vint history;
	vint tracktap;
};

const int MAX_PLAYERS=4;

extern tPlayer player[MAX_PLAYERS];


#endif
