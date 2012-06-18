#include "globals.h"
#include "playGfx.h"
#include "verbosity.h"
#include "message.h"

void playGfx::load(string dir, string filename)
{
	INFO(THEME,"Loading gameplay elements: %s %s\n",dir,filename);
	Stage::load(dir,filename);
	note[0]=findLayer("note0")->texid;
	note[1]=findLayer("note1")->texid;
	note[2]=findLayer("note2")->texid;
	note[3]=findLayer("note3")->texid;
	note[4]=findLayer("note4")->texid;
	notehl[0]=findLayer("notehl0")->texid;
	notehl[1]=findLayer("notehl1")->texid;
	notehl[2]=findLayer("notehl2")->texid;
	notehl[3]=findLayer("notehl3")->texid;
	notehl[4]=findLayer("notehl4")->texid;
	key[0]=findLayer("key0")->texid;
	key[1]=findLayer("key1")->texid;
	key[2]=findLayer("key2")->texid;
	key[3]=findLayer("key3")->texid;
	key[4]=findLayer("key4")->texid;
}
