/*******************************************************************
(C) 2010 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

#include "player.h"

#include "audio.h"
#include "scnGuitar.h"
#include "scnSetSongOpt.h"
#include "sfxfof.h"
#include "tappable.h"
#include "configuration.h"
#include "audiosfx.h"

tPlayer player[MAX_PLAYERS];

void tPlayer::whammyaxis(int v)
{
	int addv=abs(whammyaxislast-v);
	if (addv>50000) addv=50000;
	whammyaxislast=v;
	addwhammy(addv);
}

void tPlayer::addwhammy(int i)
{
	if (hitactive) whammy+=i;
}

void tPlayer::presskey(int evtime, int i)
{
	lastkeys[i]=1;
	if (i<5)
	{
		unhitactive(evtime);
		tapopportunity(evtime, i);
	}
	if (i>=5 && i<=6) handlehit(evtime);
	if (i==7) addwhammy(15000);
}

void tPlayer::releasekey(int evtime, int i)
{
	lastkeys[i]=0;
	if (i<5)
	{
		unhitactive(evtime);
		pulloffopportunity(evtime, i);
	}
}

void tPlayer::holdscore(int evtime)
{
// guitarscene line 334
	const int SCOREDIV=441;
	int multiplier=1+stat_streak/10;
	if (multiplier>4) multiplier=4;
	if (evtime<stat_holdaccounted) return;
	int scoreplus=multiplier*(evtime-stat_holdaccounted);
	if (whammyon) scoreplus*=2;
	score_frac+=scoreplus; //hitactive*
	stat_holdaccounted=evtime;
	score+=score_frac/SCOREDIV;
	score_hold+=score_frac/SCOREDIV;
	// slightly inaccurate
	if (whammyon) score_whammy+=score_frac/SCOREDIV/2;
	score_nomult+=(score_frac/SCOREDIV+multiplier-1)/multiplier;
	score_frac%=SCOREDIV;
}

void tPlayer::unhitactive(int evtime)
{
	if (hitactive)
		holdscore(evtime);
	hitactive=0;
}

void tPlayer::passtime(int evtime)
{
	int timeincr=evtime-timepassed;
	int j;
	timepassed=evtime;
	if (timeincr>0)
	{
		// whammy fades out
		whammy=(int) (whammy*exp(-timeincr*0.0001)-timeincr*0.01);
		if (whammy<0) whammy=0;
	}
	whammyon=hitactive && (whammy>5000) && (evtime-lasthit[5]>20000);
	while (crtnote<lane.size())
	{
		notestatusst &note=lane[crtnote];
		if (note.timestamp>evtime-44*tolerance_late) break;
		if (hitactive && !(note.flags & ENS_HASLINE)) unhitactive(note.timestamp);
		if (note.flags & ENS_HASHIT)
		{
			hitactive=0;
			stat_streak=0;
			tapmissnote();
		}
		crtnote++;
	}
	if (hitactive) holdscore(evtime);
}

int tPlayer::notematch(int idx, int strict)
{
	int j;
	int notesmatched=0;
	int notesunmatched=0;
	for (j=0; j<5; j++)
	{
		int noteon=lane[idx].noteon(j);
		if (noteon & !lastkeys[j]) return 0;
		if (!noteon & lastkeys[j])
		{
			if (strict) return 0;
			if (notesmatched) return 0;
			notesunmatched++;
		}
		if (noteon & lastkeys[j])
		{
			notesmatched++;
		}
		if (notesmatched>1 && notesunmatched) return 0;
	}
	return 1;
}

/**
 * return channel that should be used for audio playback for this player
 * @param id player id
 * @return 0=center, 1=left, 2=right
 */
int player_channel(int id)
{
	if (numplayers<2) return 0;
	if (numplayers==3 && id==1) return 0;
	if (id<numplayers/2) return 1;
	return 2;
}

void tPlayer::handlehit(int evtime)
{
	int i,found=-1,good=0,j;
	int multiplier=stat_streak/10+1;
	if (multiplier>4) multiplier=4;
	for (i=crtnote; i<lane.size(); i++)
	{
		if (lane[i].timestamp>evtime+44*tolerance_early) break;
		if (lane[i].flags & ENS_HASHIT)
		{
			// try to match anything within the window
			if (notematch(i,strictkeys))
			{
				found=i;
				crtnote=i+1;
				good=1;
				break;
			}
		}
	}
	if (!good & notetapped)
	{
		// if we don't get a match on the next note
		// we allow a note that was already tapped to be strummed
		for (i=crtnote-1; i>=0; i--)
		{
			if (lane[i].timestamp<evtime-44*tolerance_late) break;
			if (lane[i].flags & ENS_HASHIT)
			{
				// no harm done, but we shouldn't call
				// hitcorrect again
				if (notematch(i,strictkeys))
				{
					notetapped=0;
					return;
				}
				// only the last note
				break;
			}
		}
	}
	if (good) hitcorrect(evtime,found);
	else hitincorrect(evtime);
}

void tPlayer::hitcorrect(int evtime, int idx)
{
	int i;
	crtnote=idx+1;
	lastwasgood=1;
	maytap=1;
	notetapped=0;
	for (i=0; i<5; i++)
		if (lane[idx].noteon(i)) lasthit[i]=evtime;
	lasthit[5]=evtime;
	guitarScene.timelasthit=evtime;
	stat_streak++;
	if (stat_streak>stat_longeststreak) stat_longeststreak=stat_streak;
	hitactive=!!(lane[idx].flags & ENS_HASHIT); // ??
	stat_noteshit++;
	lane[idx].flags |= ENS_WELLPLAYED;
	score+=50*hitactive*multiplier;
	score_nomult+=50*hitactive;
	score_fromhits+=50*hitactive*multiplier;
	stat_holdaccounted=lane[idx].timestamp+8000; // actually song.period*1.1/4
	if (stat_streak==10 || stat_streak==20 || stat_streak==30) timemultiplier=evtime;
}

void tPlayer::hitincorrect(int evtime)
{
	guitarScene.timelastmiss=evtime;
	sfx_start(esfx_randombadnote(),player_channel(id),miss_loudness);
	stat_streak=0;
	lastwasgood=0;
	hitactive=0;
	if (tapmode!=ET_ANY) maytap=0;
	stat_xmiss++;
}

void player_get_music_volume(int &gtar_le, int &gtar_rt, int &rhyt_le, int &rhyt_rt, int &wham_le, int &wham_rt)
{
	if (scn.current!=SCN_PLAYING)
	{
		gtar_le=gtar_rt=85;
		wham_le=wham_rt=0;
		return;
	}
	switch (numplayers)
	{
	case 1:
		gtar_rt = gtar_le = player[0].lastwasgood?guitar_volume:guitar_off_volume;
		wham_le = wham_rt = player[0].whammyon;
		return;
	case 2:
		gtar_le = player[0].lastwasgood?guitar_volume:guitar_off_volume;
		gtar_rt = player[1].lastwasgood?guitar_volume:guitar_off_volume;
		wham_le = player[0].whammyon;
		wham_rt = player[1].whammyon;
		return;
	case 3:
		gtar_le = player[0].lastwasgood?guitar_volume:player[1].lastwasgood?(guitar_volume*2/3):guitar_off_volume;
		gtar_rt = player[2].lastwasgood?guitar_volume:player[1].lastwasgood?(guitar_volume*2/3):guitar_off_volume;
		wham_le = player[0].whammyon || player[1].whammyon ;
		wham_rt = player[2].whammyon || player[1].whammyon ;
		return;
	default:
		gtar_le = (player[0].lastwasgood?(guitar_volume/2):(guitar_off_volume/2))+(player[1].lastwasgood?(guitar_volume/2):(guitar_off_volume/2));
		gtar_rt = (player[2].lastwasgood?(guitar_volume/2):(guitar_off_volume/2))+(player[1].lastwasgood?(guitar_volume/2):(guitar_off_volume/2));
		wham_le = player[0].whammyon || player[1].whammyon ;
		wham_rt = player[2].whammyon || player[2].whammyon ;
	}
	return;
}



