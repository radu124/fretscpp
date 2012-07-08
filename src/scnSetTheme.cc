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

#include "scnSetTheme.h"

#include "configuration.h"
#include "scene.h"
#include "message.h"
#include "verbosity.h"
#include "stage.h"
#include "playGfx.h"
#include "font.h"
#include "texManager.h"
#include "fileio.h"
#include "scnGuitar.h"

tScnSetTheme scnSetTheme;

void initNeckname()
{
	int i;
	themeNeckfilenames=listDirContents(datadir+"/necks");
	themeNeckidx=0;
	themeNecknames.resize(0);
	themeNecknames.push_back("Default");
	for (i=0; i<themeNeckfilenames.size(); i++)
	{
		string s=themeNeckfilenames[i];
		themeNeckfilenames[i]="necks/"+s;
		s=s.substr(0,s.length()-4);
		themeNecknames.push_back(s);
		if (s==themeNeckname) themeNeckidx=i+1;
	}
	themeNeckname=themeNecknames[themeNeckidx];
	themeNeckfilenames.insert(themeNeckfilenames.begin(),"neck.png");
	themeNeckfilename=themeNeckfilenames[themeNeckidx];
}

void theme_init()
{
	initNeckname();
}

void tScnSetTheme::init()
{
	themes_Stage=listDirContents(datadir+"/stages",3);
	themes_Menus=listDirContents(datadir+"/menutheme",3);
	themes_Playg=listDirContents(datadir+"/playgfx",3);
	//themesScore=listDirContents(datadir+"/st_score",3);
	//themesSlist=listDirContents(datadir+"/st_slist",3);
	//themesSopts=listDirContents(datadir+"/st_sopts",3);
	//themesSettm=listDirContents(datadir+"/st_settm",3);
	additem(new tSIlist("Neck"        ,themeNecknames,&themeNeckidx));
	additem(new tSIlist("Stage"       ,themes_Stage,&selthm_Stage));
	additem(new tSIlist("Main Menu"   ,themes_Menus,&selthm_Menus));
	additem(new tSIlist("Gameplay Elements", themes_Playg, &selthm_Playg));
	additem(new tSIintrange("Neck transparency",0,90,&themeNecktransparency,10));
	tScnSetBase::init();
}

void tScnSetTheme::itemAdjusted(int i)
{
	switch(i)
	{
	case 0:
		themeNeckname=themeNecknames[themeNeckidx];
		themeNeckfilename=themeNeckfilenames[themeNeckidx];
		texRelease(sp_neck);
		sp_neck=texLoad(themeNeckfilename);
		break;
	case 1:
		stagePlay->load(datadir+"/stages/"+selthm_Stage);
		break;
	case 2:
		st_mainm ->load(datadir+"/menutheme/"+selthm_Menus, "mainmenu.ini");
		st_sopts ->load(datadir+"/menutheme/"+selthm_Menus, "startsongmenu.ini");
		st_slist ->load(datadir+"/menutheme/"+selthm_Menus, "songlist.ini");
		st_score ->load(datadir+"/menutheme/"+selthm_Menus, "scoremenu.ini");
		st_settm ->load(datadir+"/menutheme/"+selthm_Menus, "settingsmenu.ini");
		break;
	case 3:
		playgfx->cleanup();
		playgfx->load(datadir+"/playgfx/"+selthm_Playg);
		break;
	}
}

void tScnSetTheme::render()
{
	int cplayer;

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(8,0,0);
	glScalef(0.75,1,1);
	stagePlay->render();
	glPopMatrix();

	scene_setNeck(0,5);
	guitarScene.renderNeck();
	guitarScene.renderTracks();

	scene_setOrtho();
	guitarScene.activeneckpos=10;
	guitarScene.renderMultiplierVal();
	guitarScene.renderStats();

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(8,0,0);
	glScalef(0.75,1,1);
	stagePlay->renderForeground();
	glPopMatrix();

	rendertext();
}



