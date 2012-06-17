#ifndef __HDR_player_h
#define __HDR_player_h

#include "includes.h"
#include "midiparser.h"

/**
 * Set the music volume based on the number of players and each player's
 * status (correct hit/whammy)
 * @param gtar_le the left speaker guitar track volume
 * @param gtar_rt the right speaker guitar track volume
 * @param rhyt_le the left speaker rhythm track volume
 * @param rhyt_le the right speaker rhythm track volume
 * @param wham_le the left speaker whammy effect intensity (guitar track)
 * @param wham_rt the right speaker whammy effect intensity (guitar track)
 */
void player_get_music_volume(int &gtar_le, int &gtar_rt, int &rhyt_le, int &rhyt_rt, int &wham_le, int &wham_rt);

class tPlayer
{
public:
	int lefty; /// invert keys n/a
	int score; /// the current player score
	int streak; /// the number of notes correctly hit in a row
	int lastkeys[6];
	int lasthit[6]; /// time of last hit per key [5]=any
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
	/**
	 * 1 after a key was correctly hit and in the beginning
	 * used for
	 */
	int lastwasgood;
	int timemultiplier;
	/**
	 * whammy accumulates when the player repeatedly hits the whammy key
	 * or shakes the whammy bar, then it slowly fades off
	 * this is the whammy accumulator
	 */
	int whammy;
	/**
	 * from the sound effect and scoring point of view the whammy has
	 * two states: on/and off
	 * this is the indicator for the whammy state (whammy on=1)
	 */
	int whammyon;
	/**
	 * when whammy is controlled by analog input, the last value of the
	 * analog axis
	 */
	int whammyaxislast;
	/**
	 * how much of the current note they player already
	 * got credit for (timestamp)
	 */
	int holdaccounted;
	/**
	 * whether this player was enabled in the menu, before starting the song
	 */
	int enabled;
	/**
	 * the player name, from the last time high scores were entered
	 */
	string name;
	/**
	 * the last instrument that this player has used
	 */
	string lastInstr;
	/**
	 * the last difficulty that this player has used
	 */
	int lastDiffi;
	/**
	 * score without the multiplier
	 */
	int scorenomult;
	/**
	 *
	 */
	int scorehits;
	int scorehold;
	int scorewham;

	/**
	 * per player tappable mode setting, copied from global tappablemode
	 */
	int tapmode;

	/**
	 * velocity/stretching of the guitar neck
	 * 1 is the normal velocity
	 */
	GLfloat neckvelocity;

	/**
	 * the neck velocity must be initialized for layout and theme scenes
	 */
	tPlayer():neckvelocity(1)
		{;}

	/**
	 */
	void handlehit();
	/**
	 */
	void passtime();
	/**
	 * Initialize the player variables for a new game
	 * implemented in playerInit.cc
	 */
	void init(int iid);
	/**
	 */
	void presskey(int i);
	/**
	 */
	void releasekey(int i);
	/**
	 */
	void unhitactive(int ts);
	/**
	 */
	void holdscore(int ts);
	/**
	 * add to the whammy accumulator
	 */
	void addwhammy(int i);
	/**
	 * change in the analog entry for the whammy control
	 * results in an increase of the whammy accumulator
	 */
	void whammyaxis(int v);
	/**
	 * Check whether the currently pressed keys are a correct match
	 * for the note i, takes into account the global parameter
	 * strictkeys
	 * strictkeys=1 means that you are not allowed to hold the keys to
	 * the left of the played key
	 * chords are always strict
	 * @param i - the number of the note
	 * @return 1 on a correct match, 0 otherwise
	 */
	int notematch(int i);

	/**
	 * iterate through notes in the player track and set the
	 * note flags: tappable/wellplayed
	 * also sets the notetotal
	 */
	void init_note_flags();

	vector<notestatusst> lane;
};

const int MAX_PLAYERS=4;

extern tPlayer player[MAX_PLAYERS];


#endif
