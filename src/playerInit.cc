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
	longeststreak=0;
	score=0;
	scorefrac=0;
	scorenomult=0;
	scorehits=0;
	scorehold=0;
	scorewham=0;
	crtnote=0;
	hittnote=-1;
	nextnote=0;
	lane=crtSong.get_lane(instrument,difficulty);
	notetotal=0;
	notegood=0;
	notexmiss=0;
	notehopo=0;
	streak=0;
	lastwasgood=1;
	lasttime=0;
	tapmode=tappablemode;
	init_note_flags();
	// for visual effects
	timemultiplier=-1000000;
	for (i=0; i<5; i++) lasthit[i]=-1000000;
}

void tPlayer::init_note_flags()
{
	int n=lane.size();
	int i;
	for (i=0; i<n; i++)
	{
		if (!(lane[i].flags & ENS_HASHIT)) continue;
		notetotal++;
		switch (tapmode)
		{
			case ET_NONE:
				// do nothing, none of the notes is tappable
				break;
			case ET_RFMOD:
				// TODO
				break;
			case ET_GH2STRICT:
				break;
			case ET_GH2:
				break;
			case ET_GH2SLOPPY:
				break;
			case ET_ANY:
				lane[i].flags |= ENS_TAPPABLE;
				break;
		}
	}

}

