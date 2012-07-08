/*******************************************************************
(C) 2012 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

#include "flames.h"

void tFlames::clear()
{
	int i;
	for (i=0; i<MAX_FLAMES; i++)
		endts[i]=-1000000;
}

void tFlames::fireup(vector<notestatusst> *lane, int pos, int ts)
{
	int i,onefret=0;
	// this should not happen but just in case
	if (!(lane[0][pos].flags & ENS_HASHIT)) return;
	startts[0]=ts;
	mask[0]=0;
	for (i=0; i<5; i++)
		if (lane[0][pos].noteon(i))
		{
			mask[0]|=(1<<i);
			onefret=i;
		}
	// find where the flame stops on its own
	i=pos+1;
	while (lane[0][i].val[onefret]=='-') i++;
	// improperly terminated note
	if (lane[0][i].val[onefret]==' ') i--;
	endts[0]=lane[0][i].timestamp;
	// give the flame a minimum life
	if (endts[0]<ts+DEFAULT_FLAME_LIFE)
		endts[0]=ts+DEFAULT_FLAME_LIFE;
	for (i=MAX_FLAMES-1; i>0; i--)
	{
		startts[i]=startts[i-1];
		endts[i]=endts[i-1];
		mask[i]=mask[i-1];
	}
}

void tFlames::stop(int ts)
{
	int i;
	for (i=0; i<MAX_FLAMES; i++)
	{
		if (ts<endts[i]) endts[i]=ts;
		if (endts[i]<startts[i]+MIN_FLAME_LIFE)
			endts[i]=startts[i]+MIN_FLAME_LIFE;
	}
}
