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

#include "playerInit.h"

#include "player.h"
#include "scnSetSongOpt.h"
#include "configuration.h"

void tPlayer::init(int iid)
{
	int i;
	id=iid;
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
	cnotes=&crtSong.trk_notes[instrument];
	notetotal=0;
	notegood=0;
	notexmiss=0;
	notehopo=0;
	streak=0;
	lastwasgood=1;
	lasttime=0;
	for (i=0; i<cnotes->size(); i++) if (cnotes[0][i].hashit(difficulty)) notetotal++;
	history.resize(cnotes->size());
	for (i=0; i<history.size(); i++) history[i]=0;
	tappable_init_track(tappablemode);
	// for visual effects
	timemultiplier=-1000000;
	for (i=0; i<5; i++) lasthit[i]=-1000000;
}



