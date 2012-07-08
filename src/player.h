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
	/* GAMEPLAY */
	/// invert keys n/a
	int lefty;

	/* STATS */
	/**
	 * accumulated score, including all bonuses and multipliers
	 */
	int score;
	/**
	 * fractional part of score
	 */
	int score_frac;
	/**
	 * score without the multiplier
	 */
	int score_nomult;
	/**
	 * score without multiplier, can be used for computing the average
	 * multiplier at the end
	 */
	int score_fromhits;
	/**
	 * partial score due to holding down the right keys after strumming
	 * (or tapping)
	 */
	int score_hold;
	/**
	 * partial score due to using the whammy
	 */
	int score_whammy;
	/**
	 * the score multiplier 1-4
	 */
	int multiplier;
	/**
	 * the number of notes correctly hit in a row
	 */
	int stat_streak;
	/**
	 * size of the longest stat_streak
	 */
	int stat_longeststreak;
	/**
	 * Counts the correctly hit notes
	 */
	int stat_noteshit;
	/**
	 * Total number of notes on track
	 */
	int stat_notestotal;
	/**
	 * Counts additional misses, strums of non-notes
	 * a stat_xmiss different than 0 will not let the player
	 * achieve a 100% score
	 */
	int stat_xmiss;
	/**
	 * number of notes tapped
	 */
	int stat_hopos;
	/**
	 * how much of the current note they player already
	 * got credit for (timestamp)
	 */
	int stat_holdaccounted;

	/* */
	/**
	 * current keys during event
	 */
	int lastkeys[6];
	int lasthit[6]; /// time of last hit per key [5]=any
	int keys;
	int hit;
	/**
	 * Note after the one that was just played
	 */
	int crtnote;
	int nextnote;
	/**
	 * the note furthest in the future that is visible on the neck
	 * used in sgNotes.cc,
	 * TODO: move away from tPlayer
	 */
	int farrnote;
	int hittnote;
	int id;
	int crtnotehit;
	int instrument;
	int difficulty;
	int hitactive;
	/**
	 * Whether the last note was tapped rather than strummed
	 * reset by hitcorrect, set by tapopportunity
	 */
	int notetapped;
	/**
	 * time accounted for in passtime()
	 */
	int timepassed;
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
	 * per player tappable mode setting, copied from global tappablemode
	 */
	int tapmode;

	/**
	 * whether player is allowed to tap note (in some modes, tapping is
	 * disallowed after a miss
	 */
	int maytap;

	/**
	 * per player pull off enable setting, copied from global chordpulloff
	 */
	int pulloffmode;

	/**
	 * velocity/stretching of the guitar neck
	 * 1 is the normal velocity
	 */
	GLfloat neckvelocity;

	/**
	 * the neck velocity must be initialized for layout and theme scenes
	 */
	tPlayer():neckvelocity(1),maytap(1)
		{;}

	/**
	 * @param evtime time of the event in 1/44k seconds
	 */
	void handlehit(int evtime);
	/**
	 * @param evtime time of the event in 1/44k seconds
	 */
	void passtime(int evtime);
	/**
	 * Initialize the player variables for a new game
	 * implemented in playerInit.cc
	 */
	void init(int iid);

	/**
	 * the player has just pressed key i, take necessary actions:
	 *  - change status of playing note, adjust whammy, score
	 * @param evtime time of the event in 1/44k seconds
	 * @param i key id: 0-4=frets, 5-6=strum, 7=whammy
	 */
	void presskey(int evtime, int i);
	/**
	 * the player has just pressed a fret
	 * check whether this is a correct tap and adjust state accordingly
	 * @param i key id: 0-4=frets
	 * @param evtime time of the event in 1/44k seconds
	 * implemented in tappable.cc
	 */
	void tapopportunity(int evtime, int i);
	/**
	 * incorrect tap
	 */
	void tapincorrect(int evtime);

	/**
	 * an incorrect note was hit (or tapped in a tapping mode that
	 * punishes incorrect tapping by resetting the multiplier)
	 * @param evtime time of the event in 1/44k seconds
	 */
	void hitincorrect(int evtime);
	/**
	 * a note was correctly hit or tapped
	 * @param evtime time of the event in 1/44k seconds
	 * @param idx the index of the hit or tapped note
	 */
	void hitcorrect(int evtime, int idx);

	/**
	 * inform the tapping logic that a key was missed
	 */
	void tapmissnote();

	/**
	 * the player has just released a key, take necessary actions:
	 *  - change status of playing note, adjust score
	 * @param evtime time of the event
	 * @param i key id: 0-4=frets, 5-6=strum, 7=whammy
	 */
	void releasekey(int evtime, int i);
	/**
	 * the player has just released a fret
	 * implemented in tappable.cc
	 * @param evtime time of the event
	 * @param i key id: 0-4=frets
	 */
	void pulloffopportunity(int evtime, int i);

	/**
	 * end an active note after a hit because the player released the
	 * needed frets
	 * adjusts score for the time that may have elapsed
	 * @param evtime time of the event
	 * if @var hitactive is not 0, then sets @var hitactive to 0
	 */
	void unhitactive(int evtime);

	/**
	 * increment score for a key being held after strumming
	 * the time until which the score was already incremented is
	 * stored in @var holdaccounted
	 * @param evtime time of the event
	 * A fractional score is stored in @var score_frac
	 */
	void holdscore(int evtime);

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
	 * Check whether the currently pressed keys ( @var lastkeys) are a
	 * correct match for the note i
	 * takes into account the global parameter
	 * chords are always strict
	 * @param i - the number of the note
	 * @param strict - whether lower frets can be held
	 * @return 1 on a correct match, 0 otherwise
	 */
	int notematch(int i, int strict=0);

	/**
	 * iterate through notes in the player track and set the
	 * note flags: tappable/wellplayed
	 * also sets the stat_notestotal
	 */
	void init_note_flags();

	/**
	 * check if the note is tappable by the rfmod rules
	 */
	int is_rfmod_tappable(int notenum);

	vector<notestatusst> lane;
};

const int MAX_PLAYERS=4;

// + 1 dummy player for theme menus
extern tPlayer player[MAX_PLAYERS+1];


#endif
