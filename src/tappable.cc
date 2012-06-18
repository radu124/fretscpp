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

#include "tappable.h"
#include "player.h"
#include "verbosity.h"
#include "message.h"

// this is probably more complex than it should be !
int tPlayer::is_rfmod_tappable(int i)
{
	int nxt, prev;
	int j,cnt;
	// based on information posted on the Net
	nxt=i+1;
	while (nxt<lane.size())
	{
		if (lane[nxt].flags & ENS_HASHIT) break;
		nxt++;
	}
	DBG(TAPPABLE,"note: %5d is %s, next %5d ",i,lane[i].str(),nxt);
	// no next note so the answer is yes
	if (nxt>=lane.size())
	{
		DBG(TAPPABLE," tap, no next\n");
		return 1;
	}
	cnt=0;
	for (j=0; j<5; j++) cnt+=ishit(lane[nxt].val[j]);
	// The note before a chord can NOT be tapped.
	if (cnt>1)
	{
		DBG(TAPPABLE," notap, next is chord\n");
		return 0;
	}
	for (j=0; j<5; j++)
		if (ishit(lane[nxt].val[j])!=ishit(lane[i].val[j]))
		{
			// current note different from next, so answer is yes
			DBG(TAPPABLE," tap, different from next\n");
			return 1;
		}
	prev=i-1;
	while (prev>=0)
	{
		if (lane[prev].flags & ENS_HASHIT) break;
		prev--;
	}
	// no previous note so answer is false
	if (prev<0)
	{
		DBG(TAPPABLE," notap, same next, no prev\n");
		return 0;
	}
	for (j=0; j<5; j++)
		if (ishit(lane[prev].val[j])!=ishit(lane[i].val[j]))
		{
			// current note different from next, so answer is no
			DBG(TAPPABLE," notap, same next, different prev\n");
			return 0;
		}
	DBG(TAPPABLE," tap, same next, same prev\n");
	return 1;
	// The first note in a string of the same notes can NOT be tapped.
}
