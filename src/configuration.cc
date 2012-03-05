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

#include "configuration.h"

#include "fileio.h"
#include "scnSetTheme.h"
#include "video.h"
#include "message.h"
#include "verbosity.h"

string configFile;
string scoresFile;
string homeconfdir;
string datadir;
// just decided namespaces are evil
// namespaces allow you to have two things with the same name
// why would you want that, just to create confusion?
// namespace config

// for selection of the neck we have the following variables
// the filename of the neck
string themeNeckfilename;
// the index in the list of necks
int themeNeckidx;
// the config name of the neck: themeNeckname
vstring themeNecknames;
vstring themeNeckfilenames;
vstring themeStages;
vstring themesMainm;
vstring themesScore;
vstring themesSlist;
vstring themesSopts;
vstring themesSettm;


vector<tKeydefset> keydefs;
vstring keyconfname;

long keydefselector[8];
void init_config();

int conf_stretch=0;

vector<tNeckpos> neckpos;
vector<string> config_unknown;

const char *difficultynames[4]={"SuperE","Easy","Medium","Hard"};

tNeckpos defneckpos[11]={
{0.000,0.000,1.000,1.000,0.000,0},
{-3.250,0.000,1.000,0.700,-0.220,0},
{3.250,0.000,1.000,0.700,0.220,0},
{-3.500,-0.250,0.800,0.600,-0.150,0},
{0.000,2.250,0.900,0.500,0.000,0},
{3.500,-0.250,0.800,0.600,0.150,1},
{-3.000,-1.250,0.700,1.000,0.020,0},
{-2.250,2.500,0.900,0.500,-0.310,0},
{2.250,2.500,0.900,0.500,0.310,1},
{3.500,-1.250,0.700,1.000,-0.030,1},
{2.000,0.000,1.000,0.900,0.000,1}
};

#define CONFIGITEM CI_DECLARE
#define CONFIGARRAY(a,b,c,d)
CONFIGLIST
#undef CONFIGARRAY
#undef CONFIGITEM


int tsimatch(char *&s, const char *v)
{
	int l=strlen(v);
	if (0!=strncmp(s,v,l)) return 0;
	char ch=s[l];
	if (ch!='=' && ch!=' ' && ch!='\t') return 0;
	s+=l;
	while (*s==' ' || *s=='\t' || *s=='=') s++;
	DBG(CONFIG, "Matched id: %s" ,v);
	return 1;
}

int tsamatch(char *&s, const char *prefix, const char *suffix, int &idx)
{
	char *ss=s;
	int idxv;
	int l=strlen(prefix);
	if (0!=strncmp(s,prefix,l)) return 0;
	ss+=l;
	if (*ss!='[') return 0;
	idxv=0;
	ss++;
	while (*ss>='0' && *ss<='9')
	{
		idxv=idxv*10+*ss-'0';
		ss++;
	}
	if (*ss!=']')
	{
		WARN(CONFIG,"Config: expecting ]");
		return 0;
	}
	ss++;
	l=strlen(suffix);
	if (l!=0 && 0!=strncmp(ss,suffix,l)) return 0;
	ss+=l;
	if (*ss!='=' && *ss!=' ' && *ss!='\t') return 0;
	while (*ss==' ' || *ss=='\t' || *ss=='=') ss++;
	idx=idxv;
	s=ss;
	DBG(CONFIG, "Matched array: %s[%d]%s" ,prefix ,idx ,suffix);
	return 1;
}

int defkeys[]={ SDLK_F1, SDLK_F2, SDLK_F3, SDLK_F4, SDLK_F5, SDLK_RETURN, SDLK_RSHIFT, SDLK_TAB, 'p'};
int defkey2[]={ '1','2','3','4','5','k','m','l','p' };

int config_read()
{
	char line[1024];
	int i, j, v, numkeys;
	FILE *fc=fopen(configFile.c_str(),"r");
	if (!fc) return 0;
	// ini namespace marked with [ ]
	string nsp="";
	while (!feof(fc))
	{
		if (!fgets(line,999,fc)) WARN(CONFIG,"Error reading config file\n");
		char *p, *sav;
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
			nsp=sav;
			DBG(CONFIG,"Config namespace: %s\n" ,nsp);
			continue;
		}
		if (*p==';')
			continue;
		// anything outside [fretscpp] is ignored
		if (nsp!="fretscpp") continue;
		sav=p;
		DBG(CONFIG,"CONFIG: %s %s\n" ,nsp ,p);
#define CONFIGITEM CI_READ
#define CONFIGARRAY CIA_READ
CONFIGLIST
#undef CONFIGARRAY
#undef CONFIGITEM
		if (sav==p) config_unknown.push_back(sav);
	}
	fclose(fc);
	if (numkeys<2) return 0;
	return 1;
}

void config_write()
{
	int i,j;
	FILE *fou=fopen(configFile.c_str(),"w");
	if (!fou) return;
	fprintf(fou,"[fretscpp]\n");
#define CONFIGITEM CI_PRINT
#define CONFIGARRAY CIA_PRINT
CONFIGLIST
#undef CONFIGARRAY
#undef CONFIGITEM
	if (config_unknown.size()) fprintf(fou,"; unknown entries");
	for (i=0; i<config_unknown.size(); i++)
		fprintf(fou,"%s\n",config_unknown[i].c_str());
	fclose(fou);
}

void config_defaults()
{
	int i;
	keydefs.resize(0); // make sure it's clear
	keydefs.resize(2);
	keyconfname.resize(0);
	keyconfname.push_back("Keyboard");
	keyconfname.push_back("Keyb#2");
	for (i=0; i<9; i++)
	{
		keydefs[0].key[i]=defkeys[i];
		keydefs[1].key[i]=defkey2[i];
	}
	keydefselector[0]=1;
	keydefselector[1]=2;
	neckpos.resize(11);
	for (i=0; i<11; i++) neckpos[i]=defneckpos[i];
}

void config_fix()
{
	keyconfname.resize(keydefs.size());
}

void trydatadir(string td)
{
	if (datadir!="") return;
	MESSAGE("Looking for game data in: %s\n", td);
	if (fileExists(td+"/default.ttf"))
	{
		datadir=td;
		MESSAGE("found!\n");
	}
}

void init_config()
{
	int i;
	//findvideomodes();

#ifdef _WINDOWS
	_TCHAR homefolder[MAX_PATH];
	configFile="fretscpp.ini";
	homeconfdir="";
	i=SHGetFolderPath(NULL,CSIDL_PERSONAL,NULL,0/*SHGFP_TYPE_CURRENT*/,homefolder);
	if (i!=S_OK) MESSAGE("Could not get home folder\n");
	else homeconfdir=string(homefolder)+"/fretscpp";
	CreateDirectory(homeconfdir.c_str(),NULL);
	datadir="";
#else
	homeconfdir=getenv("HOME");
	homeconfdir+="/.config/fretscpp";
	uu=system((string("mkdir -p ")+homeconfdir).c_str());
#endif
	configFile=homeconfdir+"/fretscpp.ini";
	scoresFile=homeconfdir+"/scores.txt";
	trydatadir(homeconfdir+"/data");
	trydatadir("./data");
	trydatadir("/usr/share/games/fretscpp/data");
	trydatadir("/usr/local/share/games/fretscpp/data");
	trydatadir("/usr/share/games/fretsonfire/data");
	config_defaults();
	config_read();
	config_fix();
	theme_init();
}



