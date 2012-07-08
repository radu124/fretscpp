
#ifndef __HDR_FLAMES_H
#define __HDR_FLAMES_H

#include "midiparser.h"

/**
 * number of flames that may be active at the same time for one player
 * this may be subject to theme limitations
 */
const int MAX_FLAMES=8;

/**
 * The minimum life of a flame, even if the player screws up
 * about 10ms
 */
const int MIN_FLAME_LIFE=4410;

/**
 * The default life of a flame, when the note is too short
 * about 25ms
 */
const int DEFAULT_FLAME_LIFE=11025;

/**
 * This module is in charge of flame administration
 * as opposed to flame display which is done in sgFlames.cc
 */
class tFlames
{
public:
	/**
	 * time when the flame is activated, in 1/44k second units
	 */
	int startts[MAX_FLAMES];
	/**
	 * time the flame starts to fade
	 * -1000000 is a marker for an inactive flame
	 */
	int endts[MAX_FLAMES];
	/**
	 * 5-bit mask for of affected frets
	 */
	int mask[MAX_FLAMES];

	void fireup(vector<notestatusst> *lane, int pos, int ts);
	void stop(int ts);
	void clear();
};

#endif
