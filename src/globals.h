#ifndef __HDR_globals_h
#define __HDR_globals_h


#include "includes.h"
#include "midiparser.h"
#include "songini.h"

extern GLfloat scr_lrlim;
extern GLfloat scr_tblim;
extern string selectedsong;
extern int numplayers;
extern int glfontaa;
extern GLint maxTexSize;
extern SDL_Surface* scrsurface;
extern int fullscreenmode;

extern MidiParser crtSong;
extern tSongini   crtIni;


class Stage;
class playGfx;

extern Stage *stagePlay;
extern Stage *st_mainm;
extern Stage *st_sopts;
extern Stage *st_slist;
extern Stage *st_score;
extern Stage *st_settm;
extern playGfx *playgfx;

extern mcolor C_WHITE;
extern mcolor C_BLACK;
extern mcolor C_TRANSPARENT;

extern SDL_Rect **videomodesdl;
extern const SDL_VideoInfo *origmode;
extern vstring videomodenames;
extern int redefining;

struct tConfigurable
{
	int idx;
	string name;
};

struct tVideoMode:public tConfigurable
{
	int height,width,bpp;
};

extern vector<tConfigurable*> videomodes;
extern tVideoMode *fsvideomode;


#endif
