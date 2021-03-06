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

// this file implements the key rendering functions
// for the tSceneGuitar class declared in scnGuitar.h

#include "scnGuitar.h"
#include "sprites.h"
#include "texManager.h"
#include "tappable.h"
#include "player.h"
#include "playGfx.h"

void tSceneGuitar::renderKey(int col, int flags)
{
	glPushMatrix();
	glTranslatef(col-2,notePos(-1),0);
	glScalef(0.5,0.5,0.5);
	if (flags & 1)
	{
		if (flags & 2) playgfx->keystrummed[col]->render();
		else playgfx->keypressed[col]->render();
	} else playgfx->key[col]->render();

	glPopMatrix();
}

void tSceneGuitar::renderKeys()
{
	int i;
	for (i=0; i<5; i++)
		renderKey(i,pp->lastkeys[i] | pp->lastkeys[5]*2);
}



