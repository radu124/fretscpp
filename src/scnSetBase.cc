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

#include "scnSetBase.h"

#include "configuration.h"
#include "message.h"
#include "verbosity.h"
#include "stage.h"
#include "font.h"

int tSIlist::capval()
{
	int res=intcap(0,choices.size()-1,*val);
	if (res) *sval=choices[*val];
	return res;
}

int tSIclist::capval()
{
	int res=intcap(0,choices.size()-1,idx);
	if (res) *val=choices[idx];
	return res;
}

int tSettingsItem::intcap(int min, int max, int &val)
{
	DBG(SETSCENE,"Setting cap %s min:%d max:%d val:%d ", title, min, max, val);
	if (val<min)
	{
		DBG(SETSCENE,"low cap\n");
		val=min;
		return 0;
	}
	if (val>max)
	{
		DBG(SETSCENE,"high cap\n");
		val=max;
		return 0;
	}
	DBG(SETSCENE,"ok\n");
	return 1;
}

string tSIcheckbox::toString()
{
	string result;
	result="[";
	if (*val) result+="v";
	else result+=" ";
	result+="] ";
	result+=title;
	return result;
}

string tSIrange::toString()
{
	char a[32];
	string result="<-- ";
	result+=title;
	sprintf(a,": %0.2f",*val);
	result+=a;
	result+=" -->";
	return result;
}

string tSIlist::toString()
{
	char a[32];
	string result;
	if (*val>0) result+="< ";
	else result+="  ";
	result+=title+": ";
	result+=choices[*val];
	if (*val<choices.size()-1) result+=" >";
	return result;
}

string tSIclist::toString()
{
	char a[32];
	string result;
	if (idx>0) result+="< ";
	else result+="  ";
	result+=title+": ";
	result+=choices[idx]->name;
	if (idx<choices.size()-1) result+=" >";
	return result;
}

string tSIintrange::toString()
{
	char a[32];
	string result="<-- ";
	result+=title;
	sprintf(a,": %d",*val);
	result+=a;
	result+=" -->";
	return result;
}


int tSIcheckbox::handleevent(SDL_Event &event)
{
	if (event.type!=SDL_KEYDOWN) return 0;
	if (event.key.keysym.sym!=SDLK_RETURN) return 0;
	*val=!*val;
	return 1;
}


int tSIintrange::handleevent(SDL_Event &event)
{
	if (event.type!=SDL_KEYDOWN) return 0;
	switch(event.key.keysym.sym)
	{
	case SDLK_LEFT:
		if (*val<=min) return 0;
		*val-=step;
		if (*val<min) *val=min;
		return 1;
	case SDLK_RIGHT:
		if (*val>=max) return 0;
		*val+=step;
		if (*val>max) *val=max;
		return 1;
	default:;
	}
	return 0;
}

int tSIlist::handleevent(SDL_Event &event)
{
	if (event.type!=SDL_KEYDOWN) return 0;
	//MESSAGE("choices: %d %d\n" &*val &choices.size());

	switch(event.key.keysym.sym)
	{
	case SDLK_LEFT:
		*val-=1;
		return capval();
	case SDLK_RIGHT:
		*val+=1;
		return capval();
	default:;
	}
	return 0;
}

int tSIclist::handleevent(SDL_Event &event)
{
	if (event.type!=SDL_KEYDOWN) return 0;
	//MESSAGE("choices: %d %d\n" &*val &choices.size());

	switch(event.key.keysym.sym)
	{
	case SDLK_LEFT:
		idx--;
		return capval();
	case SDLK_RIGHT:
		idx++;
		return capval();
	default:;
	}
	return 0;
}

int tSIrange::handleevent(SDL_Event &event)
{
	if (event.type!=SDL_KEYDOWN) return 0;
	switch(event.key.keysym.sym)
	{
	case SDLK_LEFT:
		*val-=step;
		return capval();
	case SDLK_RIGHT:
		*val+=step;
		return capval();
	default:;
	}
	return 0;
}

void tScnSetBase::init()
{
	// TOFIX: this will create trouble during song play
	SDL_EnableKeyRepeat(133,33);
	selected=0;
	viewOffset=0;
	viewSize=12;
	posx=-38;
	posy=-6;
	scale=3;
	sceneBase::init();
}

void tScnSetBase::additem(tSettingsItem *a)
{
	item.push_back(a);
}

void tScnSetBase::handleevent(SDL_Event &event)
{
	if (event.type==SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
			case SDLK_ESCAPE:
			case SDLK_q:
				itemAdjusted(-1);
				popscene();
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
			//case SDLK_RETURN:
			default:
				if (item[selected]->handleevent(event))
					itemAdjusted(selected);
				break;
		}
	}
	// fix view offset
	if (selected<viewOffset) viewOffset=selected;
	if (viewOffset+viewSize-1<selected) viewOffset=selected+1-viewSize;
}

void tScnSetBase::render()
{
	st_settm->render();
	rendertext();
	st_settm->renderForeground();
}

void tScnSetBase::rendertext()
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
		glRotatef(v*45,0.0,0.0,1.0);
		glColor3f(0.0f,0.0f,0.0f);
		GLfloat dy=0;
		if (selected==i && scn.menufade==0)
			dy=sin(scn.time*8)*scale*0.1;
		string itemstring=item[i]->toString();
		//MESSAGE("item:%s %f %f %f\n" &itemstring &x &(y+dy) &scale);
		deffont.displayString(itemstring.c_str(),x-(v/4)*scale+scale*0.05,y+scale*0.05+dy,scale);

		if (selected==i && scn.menufade==0)
			glColor3f(1.0f,0.7f,0.0f);
		else {
			glColor3f(0.8f,0.3f,0.0f);
		}

		deffont.displayString(itemstring.c_str(),x-(v/4)*scale           ,y+dy           ,scale);
		v=v*2;
		y=y+scale;
		if (v>4) break;
	}
}




