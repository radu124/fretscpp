/*******************************************************************
(C) 2010 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

#include "globals.h"
#include "stage.h"

Stage *stagePlay;
Stage *st_mainm;
Stage *st_sopts;
Stage *st_slist;
Stage *st_score;
Stage *st_settm;
playGfx *playgfx;

MidiParser crtSong;
tSongini   crtIni;

GLfloat scr_lrlim;
GLfloat scr_tblim;
string selectedsong="";
int numplayers=1;
int glfontaa=GL_LINEAR;
GLint maxTexSize;
SDL_Surface* scrsurface;
int fullscreenmode=0;

mcolor C_WHITE(1,1,1,1);
mcolor C_BLACK(0,0,0,1);
mcolor C_TRANSPARENT(0,0,0,1);

SDL_Rect **videomodesdl;
const SDL_VideoInfo *origmode;
vstring videomodenames;
int redefining;

vector<tConfigurable*> videomodes;
tVideoMode *fsvideomode;


