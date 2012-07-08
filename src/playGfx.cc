#include "globals.h"
#include "playGfx.h"
#include "verbosity.h"
#include "message.h"

void playGfx::load(string dir, string filename)
{
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
	key[0]=findElem("key0")->texid;
	key[1]=findElem("key1")->texid;
	key[2]=findElem("key2")->texid;
	key[3]=findElem("key3")->texid;
	key[4]=findElem("key4")->texid;
}
