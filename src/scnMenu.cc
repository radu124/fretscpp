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

#include "scnMenu.h"

#include "utils.h"
#include "configuration.h"
#include "font.h"

void tScnMenuBase::enter()
{
	sceneBase::enter();
	SDL_EnableKeyRepeat(133,33);
}

void tScnMenuBase::init()
{
	selected=0;
	viewOffset=0;
	viewSize=6;
	sceneBase::init();
}

void tScnMenuBase::additem(const char *v)
{
	item.push_back(string(v));
}

void tScnMenuBase::renderback()
{

}

void tScnMenuBase::handleevent(SDL_Event &event)
{
	if (event.type==SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
			case SDLK_ESCAPE:
			case SDLK_q:
				itemClicked(-1);
				break;
			case SDLK_RETURN:
				itemClicked(selected);
				break;
			case SDLK_UP:
				if (selected>0) selected--;
				break;
			case SDLK_DOWN:
				if (selected<item.size()-1) selected++;
				break;
			case SDLK_HOME:
				selected=0;
				break;
			case SDLK_END:
				selected=item.size()-1;
				break;
			case SDLK_PAGEUP:
				selected-=viewSize;
				if (selected<0) selected=0;
				break;
			case SDLK_PAGEDOWN:
				selected+=viewSize;
				if (selected>=item.size()) selected=item.size()-1;
				break;
			default:;
		}
	}
	// fix view offset
	if (selected<viewOffset) viewOffset=selected;
	if (viewOffset+viewSize-1<selected) viewOffset=selected+1-viewSize;
}

void tScnMenuBase::render()
{
	renderback();
	rendertext();
}

void tScnMenuBase::rendertext()
{
	faderot=scn.fade*2-1;
	if (faderot<0) faderot=0;

	if (scn.menufade==1) return;
	GLfloat v,x,y,h,w;
	h=scr_height;
	w=scr_width;
	x=posx; y=posy;
	v=faderot*faderot;
	int i;
	for (i=viewOffset; (i<viewOffset+viewSize) && i<item.size(); i++)
	{
		glLoadIdentity();
		glRotatef(v*30,0.0,0.0,1.0);
		glColor3f(0.0f,0.0f,0.0f);
		GLfloat dy=0;
		if (selected==i && scn.menufade==0)
			dy=sin(scn.time*8)*scale*0.1;
		deffont.displayString(item[i].c_str(),x-(v/4)*scale+scale*0.05,y+scale*0.05+dy,scale);

		if (selected==i && scn.menufade==0)
			glColor3f(1.0f,0.7f,0.0f);
		else {
			glColor3f(0.8f,0.3f,0.0f);
		}

		deffont.displayString(item[i].c_str(),x-(v/4)*scale           ,y+dy           ,scale);
		v=v*2;
		y=y+scale;
		if (v>4) break;
	}
}




