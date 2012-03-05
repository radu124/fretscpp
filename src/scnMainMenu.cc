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

#include "scnMainMenu.h"
#include "stage.h"
#include "configuration.h"
#include "font.h"

tScnMenuMain MainMenu;

void tScnMenuMain::init()
{
	additem("Start game");
	additem("Multiplayer");
	additem("Settings");
	additem("Define controls");
	additem("Quit");
	posx=-24;
	posy=-6;
	scale=4.8;
	tScnMenuBase::init();
}

void tScnMenuMain::itemClicked(int i)
{
	switch (i)
	{
	case -1:
	case 4:
		gameover = 1;
		break;
	case 0:
		numplayers=1;
		fadetoscene(2);
		break;
	case 1:
		numplayers=2;
		fadetoscene(2);
		break;
	case 2:
		pushscene(SCN_SETTINGS);
		break;
	case 3:
		fadetoscene(SCN_KEYDEF);
		break;
	}
}

void tScnMenuMain::render()
{
	st_mainm->render();
	rendertext();
	st_mainm->renderForeground();
}




