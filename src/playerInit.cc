/*******************************************************************
(C) 2011 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

// This file implements the tPlayer::init() function of
// the tPlayer class declared in player.h

#include "player.h"
#include "scnSetSongOpt.h"
#include "configuration.h"
#include "tappable.h"

void tPlayer::init(int iid)
{
	int i;
	id=iid;
	neckvelocity=default_neck_velocity;
	whammy=0;
	stat_longeststreak=0;
	score=0;
	score_frac=0;
	score_nomult=0;
	score_fromhits=0;
	score_hold=0;
	score_whammy=0;
	stat_hopos=0;
	crtnote=0;
	hittnote=-1;
	nextnote=0;
	lane=crtSong.get_lane(instrument,difficulty);
	stat_notestotal=0;
	stat_noteshit=0;
	stat_xmiss=0;
	notetapped=0;
	stat_streak=0;
	lastwasgood=1;
	timepassed=0;
	tapmode=tappablemode;
	maytap=1;
	pulloffmode=chordpulloff;
	init_note_flags();
	// for visual effects
	timemultiplier=-1000000;
	for (i=0; i<5; i++) lasthit[i]=-1000000;
}

void tPlayer::init_note_flags()
{
	int i;
	int istappable;
	for (i=0; i<lane.size(); i++)
	{
		if (!(lane[i].flags & ENS_HASHIT)) continue;
		stat_notestotal++;
		istappable=1;
		if (tapmode==ET_NONE) continue;
		if (tapmode==ET_RFMOD)
			istappable=is_rfmod_tappable(i);
		if (istappable) lane[i].flags |= ENS_TAPPABLE;
	}

}
