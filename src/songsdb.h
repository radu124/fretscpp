#ifndef __HDR_songsdb_h
#define __HDR_songsdb_h

#include "includes.h"

/**
 * Store a song entry in the songs database
 */
struct songentry
{
	/// the song name as it will appear to the user
	string name;
	/// the directory where the song is found
	string dir;
	/// whether this is a song or a dummy entry
	int issong;
	songentry(string vname, string vdir="", int vissong=0)
		:name(vname)
		,dir(vdir)
		,issong(vissong)
		{;}
};

/**
 * The songs database, a vector of songentries
 */
extern vector<songentry> songsdb;

/**
 * Update the songs database by looking into some locations where
 * songs may be found:
 *
 * <FRETSCPP-CONFIG-DIR>/data/songs
 * /usr/share/games/fretsonfire/data/songs
 * /usr/share/games/fretscpp/data/songs
 * <CURRENT-DIR>/data/songs
 */
void updatesongdb();


#endif
