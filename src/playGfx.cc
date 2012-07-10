#include "globals.h"
#include "playGfx.h"
#include "verbosity.h"
#include "message.h"

tStageElem *playGfx::ffallback(const char *preferred, const char *fallback, int num)
{
	char name1[64];
	char name2[64];
	sprintf(name1,preferred,num);
	sprintf(name2,fallback,num);
	int v=-1,i;
	if (name1[0])
		for (i=0; i<elem.size(); i++)
			if (elem[i]->name==name1) v=i;
	if (v>=0) return elem[v];
	// create if necessary
	return findElem(name2);
}


void playGfx::load(string dir, string filename)
{
	int i;
	INFO(THEME,"Loading gameplay elements: %s %s\n",dir,filename);
	Stage::load(dir,filename);
	note[0]=findElem("note0")->texid;
	note[1]=findElem("note1")->texid;
	note[2]=findElem("note2")->texid;
	note[3]=findElem("note3")->texid;
	note[4]=findElem("note4")->texid;
	notehl[0]=findElem("notehl0")->texid;
	notehl[1]=findElem("notehl1")->texid;
	notehl[2]=findElem("notehl2")->texid;
	notehl[3]=findElem("notehl3")->texid;
	notehl[4]=findElem("notehl4")->texid;
	for (i=0; i<5; i++)
	{
		key[i]=ffallback("","key%d",i);
		keypressed[i]=ffallback("keypressed%d","key%d",i);
		keystrummed[i]=ffallback("keystrummed%d","key%d",i);
	}
}
