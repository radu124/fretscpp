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

#include "scnSetGameplay.h"

#include "configuration.h"
#include "scene.h"
#include "message.h"
#include "verbosity.h"

tScnSetGameplay scnSetGameplay;

void tScnSetGameplay::init()
{
	MESSAGE("Initialized scene: SetGameplay\n");
	additem(new tSIrange("Early tolerance",20,500,&tolerance_early,10));
	additem(new tSIrange("Late tolerance",20,500,&tolerance_late,10));
	additem(new tSIrange("Neck velocity",0.2,10,&default_neck_velocity,0.1));
	tappablemodes.push_back("None");
	tappablemodes.push_back("RF-Mod");
	tappablemodes.push_back("GH2-Strict");
	tappablemodes.push_back("GH2");
	tappablemodes.push_back("GH2-Sloppy");
	tappablemodes.push_back("Any");
	additem(new tSIlist("Tappable",tappablemodes,&tappablemode));
	additem(new tSIcheckbox("Strict single frets",&strictkeys));
	additem(new tSIintrange("Guitar volume",0,150,&guitar_volume,10));
	additem(new tSIintrange("Guitar off volume",0,150,&guitar_off_volume,10));
	tScnSetBase::init();
}



