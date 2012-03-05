/*******************************************************************
(C) 2011 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

#include "includes.h"
#include "sfxfof.h"
#include "configuration.h"
#include "audiosfx.h"

vint badnoteeffects;

int esfx_randombadnote()
{
	int q=badnoteeffects.size();
	if (!q) return 0;
	return badnoteeffects[rand()%q];
}

void load_all_sfx()
{
	badnoteeffects.push_back(sfx_load(datadir+"/fiba1.ogg"));
	badnoteeffects.push_back(sfx_load(datadir+"/fiba2.ogg"));
	badnoteeffects.push_back(sfx_load(datadir+"/fiba5.ogg"));
	badnoteeffects.push_back(sfx_load(datadir+"/fiba6.ogg"));
}



