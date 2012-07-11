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

#include "texManager.h"
#include "message.h"
#include "verbosity.h"
#include "configuration.h"
#include "vpxplayer.h"
#include "scene.h"

vector<sprite *> tmag;

int texLoad(const char *c)
{
	return texLoad(string(c));
}

int texLoad(string s, int flags)
{
	int i,found=-1;
	if (!(flags&1)) s=datadir+"/"+s;
	for (i=0; i<tmag.size(); i++)
		if (tmag[i]->name==s)
		{
			found=i;
			INFO(TEXMAN,"Texture %s already in database, entry #%d\n" ,s ,found);
			break;
		}
	if (found<0)
	{
		found=tmag.size();
		tmag.push_back(new sprite());
		tmag[found]->name=s;
		tmag[found]->tex=0;
		tmag[found]->refcount=0;
	}
	tmag[found]->refcount++;
	if (!tmag[found]->tex)
	{
		INFO(TEXMAN,"Loading %s as #%d\n" ,s ,found);
		tmag[found]->loadInternal(s);
	}
	return found;
}

void texRelease(int i)
{
	if (i<0) return;
	tmag[i]->refcount--;
	if (tmag[i]->refcount<0)
	{
		tmag[i]->refcount=0;
		WARN(TEXMAN,"Texture refcount<0: %s\n" ,tmag[i]->name);
	}
}

void texBind(int i)
{
	if (i<0) return;
	glBindTexture(GL_TEXTURE_2D,tmag[i]->tex);
	tmag[i]->boundframesago=0;
}

void texBindGetSize(int i, GLfloat &he, GLfloat &wi)
{
	if (i<0) return;
	glBindTexture(GL_TEXTURE_2D,tmag[i]->tex);
	he=tmag[i]->txhe;
	wi=tmag[i]->txwi;
	tmag[i]->boundframesago=0;
}

void texUnbind()
{
	glBindTexture(GL_TEXTURE_2D,0);
}

void texDraw(int i)
{
	if (i<0) return;
	GLfloat txhe=tmag[i]->txhe;
	GLfloat txwi=tmag[i]->txwi;
	texBind(i);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0 , txhe); glVertex3f(-1, -1, 0);
	glTexCoord2f(txwi, txhe); glVertex3f( 1, -1, 0);
	glTexCoord2f(0.0 , 0   ); glVertex3f(-1,  1, 0);
	glTexCoord2f(txwi, 0   ); glVertex3f( 1,  1, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
}

GLfloat texAspect(int i)
{
	if (i<0) return 1;
	return GLfloat(tmag[i]->height)/tmag[i]->width;
}

void texReleaseAll()
{
	int i;
	for (i=0; i<tmag.size(); i++)
		tmag[i]->release();
}

void texReloadAll()
{
	int i;
	for (i=0; i<tmag.size(); i++)
	{
		tmag[i]->tex=0;
		if (!tmag[i]->refcount)
			continue;
		tmag[i]->loadInternal(tmag[i]->name);
	}
}

void texStepAVIs()
{
	int i;
	for (i=0; i<tmag.size(); i++)
		if (tmag[i]->isavi && tmag[i]->boundframesago<5)
		{
			tmag[i]->vp->load_next_frame(tmag[i]->tex,(uint64_t) (scn.timesc*1000000));
		}
	for (i=0; i<tmag.size(); i++)
		tmag[i]->boundframesago++;
}

