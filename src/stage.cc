/*******************************************************************
(C) 2011,2012 by Radu Stefan
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
#include "stagefx.h"

tStageElem *Stage::findElem(string name)
{
	int v=-1,i;
	for (i=0; i<elem.size(); i++)
		if (elem[i]->name==name) v=i;
	if (v<0)
	{
		tStageElem *a;
		if (name=="background") a=new tStageBackground();
		else a=new tStageElem();
		a->name=name;
		elem.push_back(a);
		DBG(STAGE,"Stage: new layer %s\n", name);
		return a;
	}
	return elem[v];
}

tStageFx *Stage::findFX(string name, string typ)
{
	int v=-1,i;
	for (i=0; i<fx.size(); i++)
		if (fx[i]->name==name) v=i;
	if (v<0)
	{
		tStageFx *a=createStageFx(typ);
		a->name=name;
		fx.push_back(a);
		DBG(STAGE,"Stage: new fx %s, type:%s\n", name, typ);
		return a;
	}
	return fx[v];
}


void Stage::load(string dir, string filename)
{
	char line[1024];
	int mode=0;
	int i;
	cleanup();
	tStageElem *crtl=NULL;
	tStageFx *crtf=NULL;
	INFO(STAGE,"Loading Stage : %s/%s\n",dir,filename);
	FILE *fc=fopen((dir+"/"+filename).c_str(),"r");
	if (!fc) return;

	while (!feof(fc))
	{
		line[0]=0;
		fgets(line,999,fc);
		if (!strlen(line)) continue;
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
		if (*p==';' || *p=='#' || *p==0)
			continue;
		if (*p=='[')
		{
			sav=++p;
			while (*p!=']' && *p) p++;
			*p=0;
			if (*sav==':')
			{
				sav++;
				fxs=strchr(sav,' ');
				const char *fxnam="unnamed";
				if (!fxs)
				{
					WARN(STAGE,"Invalid effect %s\n",line);
				}
				else
				{
					fxnam=fxs+1;
					*fxs=0;
				}

				mode=2;
				crtf=findFX(fxnam,sav);
			} else {
				mode=1;
				crtl=findElem(sav);
			}
			continue;
		}
		if (mode==1) crtl->read(p);
		if (mode==2) crtf->read(p);
	}
	for (i=0; i<elem.size(); i++) elem[i]->texid=-1;
	for (i=0; i<elem.size(); i++) if (elem[i]->lv_texture!="")
	{
		string txname=elem[i]->lv_texture;
		int pos=txname.rfind(".svg");
		if (pos!=string::npos)
			txname.replace(pos,4,".png");
		else if (txname[0]=='/') elem[i]->texid=texLoad(txname.substr(1,999));
		else elem[i]->texid=texLoad(dir+"/"+txname,1);

//		elem[i]->lv_yscale=elem[i]->lv_yscale*texAspect(elem[i]->texid);
//		elem[i]->lv_yscale*=elem[i]->lv_scale;
//		elem[i]->lv_xscale*=elem[i]->lv_scale;
	}
	/*
	 * translate comes before the other effects
	 * translate equals positioning into the stage
	 */
	for (i=0; i<elem.size(); i++) if (elem[i]->lv_xpos!="0" | elem[i]->lv_ypos!="0")
	{
		INFO(STAGE,"Applying translation to %s\n", elem[i]->name);
		string xe=string("40*(")+elem[i]->lv_xpos+")";
		string ye=string("30*(")+elem[i]->lv_ypos+")";
		elem[i]->fx.push_back(new tFXtranslate(xe,ye));
	}
	/*
	 * get pointers to the user specified effects
	 */
	for (i=0; i<elem.size(); i++) if (elem[i]->lv_effects!="")
	{
		vector<string> effects=split_string(elem[i]->lv_effects,' ');
		int j;
		for (j=0; j<effects.size(); j++)
		{
			INFO(STAGE,"Binding effect %s to %s\n",effects[j],elem[i]->name);
			elem[i]->fx.push_back(findFX(effects[j]));
		}
	}
	/*
	 * scale and rotate come at the end when they are specified directly
	 * inside the element section
	 * when specified as user-defined effects they can be anywhere
	 */
	for (i=0; i<elem.size(); i++)
	{
		char ya[32];
		if (elem[i]->name=="background") continue;
		INFO(STAGE,"Setting scale %s\n", elem[i]->name);
		sprintf(ya,"%f",texAspect(elem[i]->texid));
		string xe=string("10*(")+elem[i]->lv_xscale+")";
		string ye=string("-10*(")+elem[i]->lv_yscale+")";
		if (texAspect(elem[i]->texid)!=1) ye=ye+"*("+ya+")";
		if (elem[i]->lv_scale!="1")
		{
			ye=ye+"*("+elem[i]->lv_scale+")";
			xe=xe+"*("+elem[i]->lv_scale+")";
		}
		elem[i]->fx.push_back(new tFXscale(xe,ye));
		if (elem[i]->lv_rotate!="0")
		{
			elem[i]->fx.push_back(new tFXrotate(elem[i]->lv_rotate));
		}
	}
	for (i=0; i<elem.size(); i++) if (elem[i]->lv_children!="")
	{
		vector<string> childns=split_string(elem[i]->lv_children,' ');
		int j;
		for (j=0; j<childns.size(); j++)
		{
			INFO(STAGE,"Adding child %s to %s\n",childns[j],elem[i]->name);
			elem[i]->children.push_back(findElem(childns[j]));
		}
	}
	fclose(fc);
}

void Stage::cleanup()
{
	int i;
	for (i=0; i<elem.size(); i++)
	{
		if (elem[i]->texid>=0) texRelease(elem[i]->texid);
		delete elem[i];
	}
	elem.resize(0);
	for (i=0; i<fx.size(); i++)
	{
		delete fx[i];
	}
	fx.resize(0);
}

void Stage::render()
{
	int i;
	for (i=0; i<elem.size(); i++)
	{
		if (elem[i]->lv_foreground) continue;
		if (!(elem[i]->lv_players & (1<<(numplayers-1)))) continue;
		elem[i]->render();
	}
}

void Stage::renderForeground()
{
	int i;
	for (i=0; i<elem.size(); i++)
	{
		if (elem[i]->lv_foreground!=1) continue;
		if (!(elem[i]->lv_players & (1<<(numplayers-1)))) continue;
		elem[i]->render();
	}
}

