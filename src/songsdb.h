#ifndef __HDR_songsdb_h
#define __HDR_songsdb_h


#include "includes.h"

class songentry
{
public:
	string name;
	string dir;
	int issong;
};

extern vector<songentry> songsdb;

void updatesongdb();


#endif
