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

// this file implements the note rendering functions
// for the tSceneGuitar class declared in scnGuitar.h

#include "scnGuitar.h"
#include "playGfx.h"
#include "midiparser.h"
#include "globals.h"
#include "configuration.h"
#include "player.h"
#include "texManager.h"

GLfloat tSceneGuitar::notePos(int timestamp)
{
	GLfloat velo=pp->neckvelocity;
	if (timestamp<0) return -boardline;
	return (timestamp-timenow)/44100.0*2.2*velo-boardline;
}

void tSceneGuitar::noteRegion()
{
	int step=1048576;
	nearnote=0;
	// do a binary search to find first note
	while (step>0)
	{
		int nxt=nearnote+step;
		step/=2;
		if (nxt>=lane->size()) continue;
		if (lane[0][nxt].timestamp>timenow) continue;
		nearnote=nxt;
	}
	if (nearnote<lane->size() && lane[0][nearnote].timestamp<timenow) nearnote++;
	//INFO(SCNGUITAR,"note %d\n" &crtnote);
	for (farrnote=nearnote; farrnote<lane->size(); farrnote++)
	{
		if (lane[0][farrnote].timestamp>timenow+120000/pp->neckvelocity) break;
	}
}

int tSceneGuitar::renderNote(int col, int ts, int flags)
{
	GLfloat y,nfade;
	y=notePos(ts);
	nfade=1.0f;
	if (y>1) nfade=2-y;
	//if (pos>0) nfade=1-pos;
	if (nfade<0) nfade=0;
	if (y>2) return 1;
	int tt=0;

	//glColor4f(1,1,1,nfade);
	glPushMatrix();
	glTranslatef(col-2,y,0);
	glScalef(0.5,0.5,0.5);
	if (playgfx->notehl[col] && (flags & ENS_TAPPABLE) && pp->maytap)
		playgfx->notehl[col]->render();
	else
		playgfx->note[col]->render();

	glPopMatrix();
	return 0;
}

void tSceneGuitar::renderNoteLine(int col, GLfloat from, GLfloat to, int flags)
{
	int i;
	GLfloat c0,c1,c2;
	c0=keycolors[col][0];
	c1=keycolors[col][1];
	c2=keycolors[col][2];

	if (flags==2 && !pp->hitactive) c0=c1=c2=0.5;
	if (flags==2 && pp->hitactive)
	{
		c0=1; c1=1; c2=0;
	}
	texBind(sp_noteline);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	GLfloat lw=0.1;
	col-=2;
	glBegin(GL_TRIANGLE_STRIP);
	glColor4f(c0,c1,c2,0.7f);
	if (flags==2 && pp->hitactive)
	{
		glTexCoord2f(0.0f,1); glVertex3f(col-lw,from,0);
		glTexCoord2f(1.0f,1); glVertex3f(col+lw,from,0);
		for (i=0; i<20; i++)
		{
			if (from+i*0.2+0.2>to) break;
			GLfloat a1=0.1*sin(timenow*0.0001+i*0.3)+0.1*sin(timenow*0.0001754-i*0.3)+0.1*sin(timenow*0.0001754);
			GLfloat a2=0;
			if (pp->whammyon) a2=0.1*sin(i*0.5-timenow*0.0001);
			glTexCoord2f(0.0f,1); glVertex3f(col-lw-a1+a2,from+i*0.2,0);
			glTexCoord2f(1.0f,1); glVertex3f(col+lw+a1+a2,from+i*0.2,0);
		}
		glTexCoord2f(0.0f,0); glVertex3f(col-lw,to,0);
		glTexCoord2f(1.0f,0); glVertex3f(col+lw,to,0);
		//u = ((t - time) * -.1 + pos - time) / 64.0 + .0001
        //return (math.sin(event.number + self.time * -.01 + t * .03) + math.cos(event.number + self.time * .01 + t * .02)) * .1 + .1 + math.sin(u) / (5 * u)

	}
	else
	{
		glTexCoord2f(0.0f,0); glVertex3f(col-lw,to,0);
		glTexCoord2f(1.0f,0); glVertex3f(col+lw,to,0);

		glTexCoord2f(0.0f,1); glVertex3f(col-lw,from,0);
		glTexCoord2f(1.0f,1); glVertex3f(col+lw,from,0);
	}
	glEnd();

	texUnbind();
}

void tSceneGuitar::renderNoteLines()
{
	GLfloat startNote[5];
	int active[5];
	int activecount=0;
	int i,j;
	startNote[0]=notePos(-1);
	notestatusst v;
	if (nearnote>0)
		v=lane[0][nearnote-1];
	for (i=0; i<5; i++)
	{
		char ch=v.val[i];
		active[i]=2*(ch=='O' || ch=='-');
		if (active[i]) activecount++;
		startNote[i]=startNote[0];
	}
	for (j=nearnote; j<farrnote; j++)
	{
		v=lane[0][j];
		GLfloat lend=notePos(v.timestamp);
		if (lend>necktop) break;
		for (i=0; i<5; i++)
		{
			char ch=v.val[i];
			if (active[i] && ch!='-')
			{
				renderNoteLine(i,startNote[i],lend,active[i]);
				active[i]=0;
			}
			if (ch=='O') {
				active[i]=1;
				startNote[i]=notePos(v.timestamp);
			}
		}
	}
	for (i=0; i<5; i++)
		if (active[i])
			renderNoteLine(i,startNote[i],necktop,active[i]);

}

void tSceneGuitar::renderNotes()
{
	int i,j;
	for (i=farrnote-1; i>=nearnote; i--)
	{
		notestatusst v=lane[0][i];
		int res;
		for (j=0; j<5; j++)
		{
			char ch=v.val[j];
			res=2;
			if (ch=='O' || ch=='B')
			{
				res=renderNote(j,v.timestamp,v.flags);
			}
			if (res) ch='!';
		}
	}
}



