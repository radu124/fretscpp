#include "player.h"

void generateDummyLane()
{
	int tt=120000;
	int randnote,i;
	vector<notestatusst> lane;
	notestatusst a;
	int cnt;
	while (tt<882000)
	{
		// Notehit
		tt=tt+4000+rand()%12000;
		a.timestamp=tt;
		a.val[0]=a.val[1]=a.val[2]=a.val[3]=a.val[4]=' ';
		cnt=0;
		do
		{
			randnote=rand()%5;
			a.val[randnote]='O';
			cnt++;
			if (cnt>3) break;
		} while (rand()%100>60);
		a.flags=ENS_HASHIT;
		if (a.val[0]+a.val[1]+a.val[2]+a.val[3]+a.val[4]>(4*' '+'O'))
			a.flags|=ENS_ISMULTI;
		lane.push_back(a);
		// Noteend
		tt=tt+4000+rand()%12000;
		a.timestamp=tt;
		a.flags=0;
		for (i=0; i<5; i++) if (a.val[i]!=' ') a.val[i]='X';
		lane.push_back(a);
	}
	a.timestamp=1000000000;
	lane.push_back(a);
	player[MAX_PLAYERS].lane=lane;
}

void updateDummyPlayer(int evtime)
{
	int i,lasthittime;
	int lastn;
	tPlayer &pp=player[MAX_PLAYERS];
	if (!pp.lane.size())
	{
		generateDummyLane();
		pp.crtnote=0;
		pp.flames.clear();
	}
	// it means we reset the timer
	if (pp.lane[pp.crtnote].timestamp>evtime+44100)
	{
		pp.crtnote=0;
		pp.flames.clear();
	}
	//lasthittime=-1;
	while (pp.crtnote<pp.lane.size() && evtime>pp.lane[pp.crtnote].timestamp)
	{
		if (pp.lane[pp.crtnote].flags & ENS_HASHIT)
			pp.flames.fireup(&pp.lane,pp.crtnote,pp.lane[pp.crtnote].timestamp);
		pp.crtnote++;
	}
	if (pp.crtnote<pp.lane.size())
	{
		pp.hitactive=!(pp.lane[pp.crtnote].flags & ENS_HASHIT);
		for (i=0; i<5; i++)
		{
			pp.lastkeys[i]=pp.lane[pp.crtnote].val[i]!=' ';
		}
	}
}

