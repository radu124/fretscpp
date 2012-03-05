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

#include "scnSettings.h"
#include "scene.h"
#include "stage.h"

tScnSettings scnSettings;

void tScnSettings::init()
{
	additem("Gameplay");
	additem("Layout");
	additem("Video");
	additem("Theme");
	additem("Return to Main Menu");
	posx=-38;
	posy=-4;
	scale=3;
	tScnMenuBase::init();
}

void tScnSettings::itemClicked(int i)
{
	switch (i)
	{
	case -1:
	case 4:
		popscene();
		break;
	case 0:
		pushscene(SCN_SETGAMEPLAY);
		break;
	case 1:
		pushscene(SCN_SETLAYOUT);
		break;
	case 2:
		pushscene(SCN_SETVIDEO);
		break;
	case 3:
		pushscene(SCN_SETTHEME);
		break;
	}
}

void tScnSettings::render()
{
	st_settm->render();
	rendertext();
	st_settm->renderForeground();
}




