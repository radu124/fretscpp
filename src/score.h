#ifndef __HDR_score_h
#define __HDR_score_h


#include "includes.h"

struct scoreline
{
	uint8_t midhash[20];
	uint32_t flags;
	uint64_t date;
	char playername[36];
	int32_t instrument;
	int32_t difficulty;
	int32_t score;
	int32_t streak;
	int32_t notestotal;
	int32_t notesgood;
	int32_t xmisses;
	int32_t noteshopo;
	int32_t scorenomult;
	int32_t scorehits;
	int32_t scorehold;
	int32_t scorewham;
	uint8_t scorehash[20];
	int read(FILE *f, int ver=1);
	void write(FILE *f);
	void readfromplayer(int num);
	string toStringDump();
	string percentage();
	int promille();
};

class tscores
{
public:
	vint best;
	vint crtids;
	vector<scoreline> data;
	vstring history;
	void load(string file);
	void setCurrent();
	void init();
};

int scorepromille(int tot, int good, int xmiss);

extern tscores scores;

#endif
