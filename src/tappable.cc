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

void tPlayer::tappable_init_track(int mode)
{
	int n=cnotes->size();
	int i;
	tracktap.resize(0);
	tracktap.resize(n);
	for (i=0; i<n; i++)
	{
		switch (mode)
		{
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
				break;
		}
	}
}


