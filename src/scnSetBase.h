#ifndef __HDR_scnSetBase_h
#define __HDR_scnSetBase_h


#include "scene.h"
#include "globals.h"

struct tSettingsItem
{
public:
	enum {TYP_NULL=0,TYP_CHECKBOX,TYP_LISTBOX,TYP_RANGE,TYP_EDITBOX};
	string title;
	int typ;
	tSettingsItem(string tv="", int ty=0):title(tv),typ(ty) {;}
	virtual int handleevent(SDL_Event &event) { return 0; }
	virtual string toString() { return ""; }
	int intcap(int min, int max, int &val);
};

struct tSIcheckbox:public tSettingsItem
{
	int *val;
	int locval;
	string toString();
	int handleevent(SDL_Event &event);
	tSIcheckbox(string t, int *v):tSettingsItem(t,TYP_CHECKBOX),val(v) { if (!v) val=&locval; }
};

struct tSIrange:public tSettingsItem
{
	tSIrange(string v, GLfloat miv, GLfloat mav, GLfloat *vv, GLfloat stv):
		tSettingsItem(v,TYP_RANGE),min(miv),max(mav),val(vv),step(stv) { if (!vv) val=&locval; capval(); }
	GLfloat *val;
	GLfloat min,max,step,locval;
	int capval() { if (*val<min) { *val=min; return 0; } if (*val>max) { *val=max; return 0; } return 1; }
	int handleevent(SDL_Event &event);
	string toString();
};

struct tSIintrange:public tSettingsItem
{
	tSIintrange(string v, int miv, int mav, int *vv, int stv=1):
		tSettingsItem(v,TYP_RANGE),min(miv),max(mav),val(vv),step(stv) { if (!vv) val=&locval; capval(); }
	int *val;
	int min,max,step,locval;
	int capval() { return intcap(min,max,*val); }
	int handleevent(SDL_Event &event);
	string toString();
};

struct tSIlist:public tSettingsItem
{
	tSIlist(string v, vector<string> cv,int *vv):
		tSettingsItem(v,TYP_RANGE),choices(cv),val(vv),sval(&locsval)
		{
			if (!cv.size()) choices.push_back("");
			if (!vv)
			{
				val=&locval;
				*val=0;
			}
			capval();
		}
	tSIlist(string v, vector<string> cv,string *vv):
		tSettingsItem(v,TYP_RANGE),choices(cv),val(&locval),sval(vv)
		{
			int i;
			if (!cv.size()) choices.push_back("");
			locsval="";
			if (!vv) sval=&locsval;
			*val=0;
			for (i=0; i<cv.size(); i++) if (*sval==cv[i]) *val=i;
			capval();
		}
	vector<string> choices;
	int *val;
	string *sval;
	string locsval;
	int locval;
	int capval();
	int handleevent(SDL_Event &event);
	string toString();
};

struct tSIclist:public tSettingsItem
{
	tSIclist(string v, vector<tConfigurable*> cv,tConfigurable **vv):
		tSettingsItem(v,TYP_RANGE),choices(cv),val(vv),idx(0)
		{
			if (!cv.size()) choices.push_back(new tConfigurable());
			if (!vv) val=&locval; capval();
		}
	vector<tConfigurable*> choices;
	tConfigurable **val;
	tConfigurable *locval;

	int idx;
	int capval();
	int handleevent(SDL_Event &event);
	string toString();
};

class tScnSetBase: public sceneBase
{
public:
	vector<tSettingsItem*> item;
	int selected;
	int viewOffset;
	int viewSize;
	GLfloat scale,posx,posy,faderot;
	void init();
	virtual void itemAdjusted(int i) {;}
	GLfloat fadespeed() { return 100; }
	void additem(tSettingsItem *i);
	void render();
	void rendertext();
	void handleevent(SDL_Event &event);
};


#endif
