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
#include "playerInit.h"
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

void tPlayer::presskey(int i)
{
	lastkeys[i]=1;
	if (i<5) unhitactive(guitarScene.timenow);
	if (i>=5 && i<=6) handlehit();
	if (i==7) addwhammy(15000);
}

void tPlayer::releasekey(int i)
{
	lastkeys[i]=0;
	if (i<5) unhitactive(guitarScene.timenow);
}

void tPlayer::holdscore(int ts)
{
// guitarscene line 334
	const int SCOREDIV=441;
	int multiplier=1+streak/10;
	int scoreplus;
	if (multiplier>4) multiplier=4;
	if (ts<holdaccounted) return;
	scoreplus=multiplier*(ts-holdaccounted);
	if (whammyon) scoreplus*=2;
	scorefrac+=scoreplus; //hitactive*
	holdaccounted=ts;
	score+=scorefrac/SCOREDIV;
	scorehold+=scorefrac/SCOREDIV;
	if (whammyon) scorewham+=scorefrac/SCOREDIV/2;
	scorenomult+=(scorefrac/SCOREDIV+multiplier-1)/multiplier; // not really good
	scorefrac%=SCOREDIV;
}

void tPlayer::unhitactive(int ts)
{
	if (hitactive)
		holdscore(ts);
	hitactive=0;
}

void tPlayer::passtime()
{
	int timenow=guitarScene.timenow;
	int timeincr=timenow-lasttime;
	int j;
	lasttime=timenow;
	if (timeincr>0)
	{
		// whammy fades out
		whammy=(int) (whammy*exp(-timeincr*0.0001)-timeincr*0.01);
		if (whammy<0) whammy=0;
	}
	whammyon=hitactive && (whammy>5000) && (timenow-lasthit[5]>20000);
	while (crtnote<crtSong.trk_notes[instrument].size())
	{
		notestatus &cn=cnotes[0][crtnote];
		if (cn.timestamp>timenow-44*tolerance_late) break;
		if (hitactive && !cn.hasline(difficulty)) unhitactive(cn.timestamp);
		if (cnotes[0][crtnote].hashit(difficulty))
		{
			hitactive=0;
			streak=0;
		}
		crtnote++;
	}
	if (hitactive) holdscore(timenow);
}

int tPlayer::notematch(int i)
{
	int j;
	int notesmatched=0;
	int notesunmatched=0;
	for (j=0; j<5; j++)
	{
		char c=cnotes[0][i].stat[j+12*difficulty];
		int noteon=(c=='O' || c=='B');
		if (noteon & !lastkeys[j]) return 0;
		if (!noteon & lastkeys[j])
		{
			if (strictkeys) return 0;
			if (notesmatched) return 0;
			notesunmatched++;
		}
		if (noteon & lastkeys[j])
		{
			notesmatched++;
		}
		if (notesmatched>1 && notesunmatched) return 0;
		//if (lastkeys[j]) c++;
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

void tPlayer::handlehit()
{
	int timenow=guitarScene.timenow;
	int i,found=-1,good=0,j;
	int multiplier=streak/10+1;
	if (multiplier>4) multiplier=4;
	for (i=crtnote; i<crtSong.trk_notes[instrument].size(); i++)
	{
		if (cnotes[0][i].timestamp>timenow+44*tolerance_early) break;
		if (cnotes[0][i].hashit(difficulty))
		{
			// try to match anything within the window
			if (notematch(i))
			{
				found=i;
				crtnote=i+1;
				good=1;
				break;
			}
		}
	}
	if (good) {
		lastwasgood=1;
		for (i=0; i<5; i++)
			if (lastkeys[i]) lasthit[i]=timenow;
		lasthit[5]=timenow;
		guitarScene.timelasthit=timenow;
		streak++;
		if (streak>longeststreak) longeststreak=streak;
		hitactive=cnotes[0][crtnote-1].hashit(difficulty);
		notegood++;
		history[crtnote-1]=2;
		score+=50*hitactive*multiplier;
		scorenomult+=50*hitactive;
		scorehits+=50*hitactive*multiplier;
		holdaccounted=cnotes[0][crtnote-1].timestamp+8000; // actually song.period*1.1/4
		if (streak==10 || streak==20 || streak==30) timemultiplier=timenow;
	}
	else {
		guitarScene.timelastmiss=timenow;
		sfx_start(esfx_randombadnote(),player_channel(id),miss_loudness);
		streak=0;
		lastwasgood=0;
		hitactive=0;
		notexmiss++;
	}
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



