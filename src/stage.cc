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

#include "stage.h"

#include "configuration.h"
#include "texManager.h"

MULTIRDR(SLBLENDTYPE,"default,one")

StageLayer::StageLayer()
{
#define SLPD SLPD_INIT
	SLINI_LIST
#undef SLPD
}

void StageLayer::read(char *line)
{
#define SLPD SLPD_READ
	SLINI_LIST
#undef SLPD
}


void StageLayer::render()
{
	int i;
	if (isBackground)
	{
		glClearColor( lv_color.Red, lv_color.Green, lv_color.Blue, 0 );
		glClear( GL_COLOR_BUFFER_BIT );
		return;
	}
	glPushMatrix();
	if (lv_src_blending) glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	color=lv_color;
	glTranslatef(40*lv_xpos,30*lv_ypos,0);
	for (i=0; i<fx.size(); i++) fx[i]->apply(color);
	glScalef(10*lv_xscale,-10*lv_yscale,0);
	if (lv_angle!=0) glRotatef(-lv_angle,0,0,1);
	color.set();
	texDraw(texid);
	if (lv_src_blending) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPopMatrix();
}

StageLayer *Stage::findLayer(string name)
{
	int v=-1,i;
	for (i=0; i<layer.size(); i++)
		if (layer[i]->name==name) v=i;
	if (v<0)
	{
		StageLayer *a=new StageLayer();
		a->name=name;
		a->isBackground=(a->name=="background");
		layer.push_back(a);
		DBG(STAGE,"Stage: new layer %s\n", name);
		a->parent=this;
		return a;
	}
	return layer[v];
}

StageLayerFx *Stage::findFX(StageLayer *base, string name)
{
	int v=-1,i;
	for (i=0; i<base->fx.size(); i++)
		if (base->fx[i]->name==name) v=i;
	if (v<0)
	{
		StageLayerFx *a=new StageLayerFx();
		a->name=name;
		base->fx.push_back(a);
		DBG(STAGE,"Stage: new fx %s (layer %s)\n", name, base->name);
		a->parent=base;
		return a;
	}
	return base->fx[v];
}


void Stage::load(string dir, string filename)
{
	char line[1024];
	int mode=0;
	int i;
	cleanup();
	StageLayer *crtl=NULL;
	StageLayerFx *crtf=NULL;
	FILE *fc=fopen((dir+"/"+filename).c_str(),"r");
	if (!fc) return;

	while (!feof(fc))
	{
		fgets(line,999,fc);
		char *p, *sav, *fxs;
		// eliminate CR,LF at the end of line
		p=line+strlen(line)-1;
		while (p>=line)
		{
			if (*p!=10 && *p!=13) break;
			*p=0;
		}
		p=line;
		while (*p==' ' || *p=='\t') p++;
		if (*p=='[')
		{
			sav=++p;
			while (*p!=']' && *p) p++;
			*p=0;
			fxs=strchr(sav,':');
			if (fxs) {
				*fxs=0;
				fxs++;
				mode=2;
				crtl=findLayer(sav);
				crtf=findFX(crtl,fxs);
			} else {
				mode=1;
				crtl=findLayer(sav);
			}
			continue;
		}
		if (*p==';')
			continue;
		if (mode==1) crtl->read(p);
		if (mode==2) crtf->read(p);
	}
	for (i=0; i<layer.size(); i++) layer[i]->texid=-1;
	for (i=0; i<layer.size(); i++) if (layer[i]->lv_texture!="")
	{
		string txname=layer[i]->lv_texture;
		int pos=txname.rfind(".svg");
		if (pos!=string::npos)
			txname.replace(pos,4,".png");
		else if (txname[0]=='/') layer[i]->texid=texLoad(txname.substr(1,999));
		else layer[i]->texid=texLoad(dir+"/"+txname,1);

		layer[i]->lv_yscale=layer[i]->lv_yscale*texAspect(layer[i]->texid);
		layer[i]->lv_yscale*=layer[i]->lv_scale;
		layer[i]->lv_xscale*=layer[i]->lv_scale;
	}
	fclose(fc);
}

void Stage::cleanup()
{
	int i;
	for (i=0; i<layer.size(); i++)
	{
		if (layer[i]->texid>=0) texRelease(layer[i]->texid);
		delete layer[i];
	}
	layer.resize(0);
}

void Stage::render()
{
	int i;
	for (i=0; i<layer.size(); i++)
	{
		if (layer[i]->lv_foreground) continue;
		if (!(layer[i]->lv_players & (1<<(numplayers-1)))) continue;
		layer[i]->render();
	}
}

void Stage::renderForeground()
{
	int i;
	for (i=0; i<layer.size(); i++)
	{
		if (!layer[i]->lv_foreground) continue;
		if (!(layer[i]->lv_players & (1<<(numplayers-1)))) continue;
		layer[i]->render();
	}
}



