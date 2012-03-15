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

#include "scnKeydef.h"

#include "sprites.h"
#include "scnTable.h"
#include "globals.h"
#include "keyboard.h"
#include "joystick.h"
#include "configuration.h"

tScnKeydef SCNKeyDef;

void tScnKeydef::enter()
{
	int res,i,j;
	redefining=0;
	makeempty();
	additem("ODone");
	additem("");
	additem("^");
	additem("^Fret #1");
	additem("^Fret #2");
	additem("^Fret #3");
	additem("^Fret #4");
	additem("^Fret #5");
	additem("^Pick");
	additem("^Alt Pick");
	additem("^Whammy");
	additem("^Fire");
	for (i=0; i<keydefs.size(); i++)
	{
		newcol();
		additem(string("O")+keydefs[i].name);
		additem("");
		additem("ODefine all");
		for (j=0; j<9; j++)
			additem(string("O")+keyname(keydefs[i].key[j]));
	}
	newcol();
	additem("OAdd New");
	fixedcols=1;
	colsvisible=2;
	baseh=fixedcols;
	optinit();
}

void tScnKeydef::handleevent(SDL_Event &a)
{
	int evt=a.type;
	int ch=a.key.keysym.sym;

	if (!redefining)
	{
		tScnTable::handleevent(a);
		return;
	}

	if (evt!=SDL_KEYDOWN) return;
	if (ch==SDLK_ESCAPE)
	{
		redefining=0;
		entries[selx][sely]=string("O")+SDL_GetKeyName((SDLKey) keydefs[selx-1].key[sely-3]);
		return;
	}
	keydefs[selx-1].key[sely-3]=ch;
	entries[selx][sely]=string("O")+keyname(ch);
	if (redefining==1 || sely==11) redefining=0;
	else {
		sely++;
		if (sely==10) joystick_initdetect();
		entries[selx][sely]="O<press key>";
	}
}

void tScnKeydef::itemClicked(int i, int j)
{
	int q;
	if (j==-1)
	{
		fadetoscene(SCN_MAINMENU);
		return;
	}
	if (j==0 && i==0)
	{
		fadetoscene(SCN_MAINMENU);
		return;
	}
	if (j==0 && i==entries.size()-1)
	{
		opt[i].resize(12);
		entries[i].resize(12);
		opt[i][0]=opt[i][1]=opt[i][2]=0;
		entries[i][0]="OAdded";
		entries[i][1]="";
		entries[i][2]="ODefine All";
		keydefs.push_back(tKeydefset());
		for (q=0; q<9; q++)
		{
			entries[i][q+3]="O-";
			keydefs.back().key[q]=0;
		}
		keydefs.back().name=entries[i][0].substr(1);
		newcol();
		additem("OAdd New");
	}
	if (j<2) return;
	if (j==2)
	{
		redefining=2;
		sely++;
	} else {
		redefining=1;
	}
	if (sely==10) joystick_initdetect();
	entries[selx][sely]="O<press key>";
}




