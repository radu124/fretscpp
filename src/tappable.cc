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
#include "configuration.h"

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

void tPlayer::tapopportunity(int evtime, int i)
{
	if (!tapmode) return;
	if (!maytap) return;
	int nxt;
	for (nxt=crtnote; nxt<lane.size(); nxt++)
	{
		if (lane[nxt].timestamp>evtime+44*tolerance_early)
		{
			nxt=lane.size();
			break;
		}
		if (lane[nxt].flags & ENS_HASHIT) break;
	}
	if (nxt>=lane.size())
	{
		// incorrect tap
		tapincorrect(evtime);
		return;
	}
	if (!(lane[nxt].flags & ENS_TAPPABLE)) return;
	if (!lane[nxt].noteon(i))
	{
		tapincorrect(evtime);
		return;
	}
	if (notematch(nxt,1))
	{
		hitcorrect(evtime,nxt);
		notetapped=1;
		stat_hopos++;
		return;
	}
	// allow tapping the notes of a chord one-by-one, thus not an error
	if (lane[nxt].flags & ENS_ISMULTI)
		return;
	// tapped a single while holding another fret
	tapincorrect(evtime);
}

void tPlayer::tapincorrect(int evtime)
{
	switch (tapmode)
	{
	case ET_RFMOD:
		maytap=0;
		break;
	case ET_GH2STRICT:
		maytap=0;
		hitincorrect(evtime);
		break;
	case ET_GH2:
		maytap=0;
		break;
	case ET_GH2SLOPPY:
		break;
	case ET_ANY:
		hitincorrect(evtime);
		break;
	}
}

void tPlayer::pulloffopportunity(int evtime, int i)
{
	// nothing to do if pull-off is disabled
	if (!pulloffmode) return;
	if (!maytap) return;
	int nxt;
	for (nxt=crtnote; nxt<lane.size(); nxt++)
	{
		if (lane[nxt].timestamp>evtime+44*tolerance_early)
		{
			nxt=lane.size();
			break;
		}
		if (lane[nxt].flags & ENS_HASHIT) break;
	}
	if (nxt>=lane.size()
		|| !(lane[nxt].flags & ENS_TAPPABLE))
		return;
	if (notematch(nxt,1)) {
		hitcorrect(evtime,nxt);
		notetapped=1;
		stat_hopos++;
	}
}

void tPlayer::tapmissnote()
{
	// not sure what do do
}
