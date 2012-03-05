#ifndef __HDR_midiparser_h
#define __HDR_midiparser_h


#include "includes.h"

struct notestatus
{
	int timestamp;
	char stat[64]; // " O-XB"
	string otherstat;
	int hashit(int difficulty);
	int hasline(int difficulty);
};

typedef vector<notestatus> vnotestatus;
typedef vector<vnotestatus> vvnotestatus;

class MidiParser
{
	unsigned char *buffer;
	int size;
	int fd;
	int pos;
	int savedpos;
	int nxmark;
	int chunkend,chunklen;
	int format;
	int tracks;
	int ticksperbeat;
	int ts_numer, ts_denom, ts_metro, ts_32nds;
	double tickduration;
	notestatus a;
	vector<notestatus> score;
protected:
	int FAIL;
	int expect(const char *str);
	int doparse();
	int isthere(const char *str);
	unsigned int readbyte();
	unsigned int readshort();
	unsigned int readlong();
	unsigned int readvarlen();
	void timeincrement(int delta);
public:
	int lastevent;
	uint8_t hash[20];
	void applydelay(int delay);
	vvnotestatus trk_notes;
	vector<int> trk_difficulties;
	vector<string> trk_instrument;

	int openfile(const char *name);
	void debug_dump_track(int track);
};


#endif
