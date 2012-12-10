#ifndef __HDR_songini_h
#define __HDR_songini_h


#include "includes.h"

#define SONGINI_LIST \
SIPD(string,artist) \
SIPD(string,icon) \
SIPD(string,name) \
SIPD(string,tags) \
SIPD(int,diff_guitar) \
SIPD(int,diff_drums) \
SIPD(int,diff_bass) \
SIPD(int,diff_band) \
SIPD(string,unlock_id) \
SIPD(string,unlock_require) \
SIPD(string,unlock_text) \
SIPD(string,casettecolor) \
SIPD(int,count) \
SIPD(string,scores) \
SIPD(string,scores_ext) \
SIPD(int,tutorial) \
SIPD(int,delay) \
SIPD(string,frets) \
SIPD(string,version) \
SIPD(int,year) \
SIPD(string,genre) \
SIPD(string,loading_phrase) \
SIPD(int,hopo_freq) \
SIPD(string,video) \
SIPD(int,video_start_time) \
SIPD(int,video_end_time) \
SIPD(int,preview_start_time) \
SIPD(string,cover) \
SIPD(string,background) \

#define SIPD_init_string(a)
#define SIPD_init_int(a) a=0;

struct tSongini
{
#define SIPD(a,b) a b;
	SONGINI_LIST
#undef SIPD
	tSongini(){
#define SIPD(a,b) SIPD_init_##a(b);
	SONGINI_LIST
#undef SIPD
		}
	int load(string filename);
};


#endif
