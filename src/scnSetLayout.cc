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

#include "scnSetLayout.h"

#include "configuration.h"
#include "scene.h"
#include "scnGuitar.h"
#include "message.h"
#include "verbosity.h"
#include "stage.h"
#include "font.h"

tScnSetLayout scnSetLayout;

void replaceSI(tSettingsItem *&old, tSettingsItem *cnew)
{
	tSettingsItem *oldv;
	oldv=old;
	old=cnew;
	delete oldv;
}

void tScnSetLayout::setTarget()
{
	int active;
	if (crtplayer>np) crtplayer=np;
	sicrtpl->max=np;
	active=np*(np-1)/2+crtplayer-1;
	assume(active>=0 && active<NECKPOS_COUNT,"trying to set wrong neckpos");
	MESSAGE("Setting target %d\n", active);
	replaceSI(item[3],new tSIrange("XPos",-20,20,&neckpos[active].xdisp,0.25));
	replaceSI(item[4],new tSIrange("YPos",-20,20,&neckpos[active].ydisp,0.25));
	replaceSI(item[5],new tSIrange("Scale",0.1,4,&neckpos[active].scale,0.1));
	replaceSI(item[6],new tSIrange("Stretch",0.1,4,&neckpos[active].stretch,0.1));
	replaceSI(item[7],new tSIrange("Skew",-2,2,&neckpos[active].skew,0.01));
	replaceSI(item[8],new tSIlist("Status position",statpositions,&neckpos[active].statpos));
}

void tScnSetLayout::itemAdjusted(int i)
{
	if (i==0 || i==1) setTarget();
}

void tScnSetLayout::init()
{
	posy=-28;
	scale=2;
	additem(new tSIintrange("Num players",1,MAX_PLAYERS,&np));
	sicrtpl=new tSIintrange("Current player",1,MAX_PLAYERS,&crtplayer);
	additem(sicrtpl);
	statpositions.push_back("Left");
	statpositions.push_back("Right");
	additem(new tSIrange("Extend Neck",-1,4,&neckpos_extend,0.2));
	additem(new tSettingsItem());
	additem(new tSettingsItem());
	additem(new tSettingsItem());
	additem(new tSettingsItem());
	additem(new tSettingsItem());
	additem(new tSettingsItem());
	setTarget();
	tScnSetBase::init();
	guitarScene.init();
}

void tScnSetLayout::render()
{
	int cplayer;
	st_settm->render();
	for (cplayer=0; cplayer<np; cplayer++)
	{
		scene_setNeck(cplayer,np);
		guitarScene.renderNeck();
		guitarScene.renderTracks();
	}
	scene_setOrtho();
	for (cplayer=0; cplayer<np; cplayer++)
	{
		guitarScene.cplayer=cplayer;
		guitarScene.activeneckpos=cplayer+np*(np-1)/2;
		guitarScene.renderMultiplierVal();
		guitarScene.renderStats();
	}
	rendertext();
}



