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
	for (i=0; i<5; i++)
	{
		note[i]=ffallback("","note%d",i);
		notehl[i]=ffallback("notehl%d","note%d",i);
		key[i]=ffallback("","key%d",i);
		keypressed[i]=ffallback("keypressed%d","key%d",i);
		keystrummed[i]=ffallback("keystrummed%d","key%d",i);
	}
	multiplierbar=findElem("multiplierbar");
}
