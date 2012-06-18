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
#include "confrw.h"
#include "fileio.h"
#include "scnSetTheme.h"
#include "video.h"
#include "message.h"
#include "verbosity.h"
#include "utils.h"

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
vstring themesPlay;


vector<tKeydefset> keydefs;

long keydefselector[MAX_PLAYERS];

vector<tNeckpos> neckpos;
vector<string> config_unknown;

/**
 * Default values for the difficulty names
 */
const char *difficultynames[4]={"SuperE","Easy","Medium","Hard"};

/**
 * Default values for the neck positions
 */
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
	// match the beginning of the string to the variable name
	if (0!=strncmp(s,v,l)) return 0;
	char ch=s[l];
	// if the next character is not space or the equal sign, this
	// is not a successful match
	if (ch!='=' && ch!=' ' && ch!='\t') return 0;
	s+=l;
	// skip additional space and the equal sign and increment
	// (the pointer) s to point to the actual value
	while (*s==' ' || *s=='\t' || *s=='=') s++;
	DBG(CONFIG, "Matched id: %s" ,v);
	return 1;
}

int tsamatch(char *&s, const char *prefix, const char *suffix, int &idx)
{
	char *ss=s;
	int idxv;
	int l=strlen(prefix);
	// match the beginning of the string to the base variable name
	if (0!=strncmp(s,prefix,l)) return 0;
	ss+=l;
	// for a match the next character has to be the opening bracket
	// of the index
	if (*ss!='[') return 0;
	idxv=0;
	ss++;
	// numeric characters will form the index value
	while (*ss>='0' && *ss<='9')
	{
		idxv=idxv*10+*ss-'0';
		ss++;
	}
	// expecting a closing bracket, print a warning if we got this
	// far but failed to find it (and bail out - no match)
	if (*ss!=']')
	{
		WARN(CONFIG,"Config: expecting ]");
		return 0;
	}
	ss++;
	// match the suffix if it exists
	l=strlen(suffix);
	if (l!=0 && 0!=strncmp(ss,suffix,l)) return 0;
	ss+=l;
	// if the character after the suffix is not space or the equal sign
	// then this is not a match (useful when one suffix is a substring
	// of another
	if (*ss!='=' && *ss!=' ' && *ss!='\t') return 0;
	// skip additional space and the equal sign
	while (*ss==' ' || *ss=='\t' || *ss=='=') ss++;
	// assign index and new s value
	idx=idxv;
	s=ss;
	DBG(CONFIG, "Matched array: %s[%d]%s" ,prefix ,idx ,suffix);
	return 1;
}

/**
 * Default keymaps for the first and second player
 */
int defkeys[]={ SDLK_F1, SDLK_F2, SDLK_F3, SDLK_F4, SDLK_F5, SDLK_RETURN, SDLK_RSHIFT, SDLK_TAB, 'p'};
int defkey2[]={ '1','2','3','4','5','k','m','l','p' };

int config_read()
{
	// configuration lines should be at most 1024 characters long
	char line[1024];
	int i, j, v, numkeys;
	FILE *fc=fopen(configFile.c_str(),"r");
	if (!fc) return 0;
	// ini namespace marked with [ ]
	string nsp="";
	// process the configuration file line-by-line
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
		// skip initial spaces
		while (*p==' ' || *p=='\t') p++;
		// match section delimiter (namespace)
		if (*p=='[')
		{
			sav=++p;
			while (*p!=']' && *p) p++;
			*p=0;
			nsp=sav;
			DBG(CONFIG,"Config namespace: %s\n" ,nsp);
			continue;
		}
		// skip comments
		if (*p==';')
			continue;
		// anything outside [fretscpp] is ignored
		if (nsp!="fretscpp") continue;
// p is modified on a successful match, save its value
				sav=p;
		DBG(CONFIG,"CONFIG: %s %s\n" ,nsp ,p);
		// expand the CONFIGLIST MACRO
		// this will try to match one-by-one all configuration values
#define CONFIGITEM CI_READ
#define CONFIGARRAY CIA_READ
CONFIGLIST
#undef CONFIGARRAY
#undef CONFIGITEM
		// in case of no match, store the unknown variable for later
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
	// expand the CONFIGLIST macro
	// to print all known variables
#define CONFIGITEM CI_PRINT
#define CONFIGARRAY CIA_PRINT
CONFIGLIST
#undef CONFIGARRAY
#undef CONFIGITEM
	// print lines from the original file that did not match
	// one of the configuration variable names
	if (config_unknown.size()) fprintf(fou,"; unknown entries");
	for (i=0; i<config_unknown.size(); i++)
		fprintf(fou,"%s\n",config_unknown[i].c_str());
	fclose(fou);
}

/**
 * Provide initial values for keydefs and neck positions
 * in case no configuration file exists
 */
void config_defaults()
{
	int i;
	keydefs.resize(0); // make sure it's clear
	keydefs.resize(2);
	keydefs[0].name="Keyboard";
	keydefs[1].name="Keyb#2";
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

/**
 * Initialize config names if they were empty
 * (old config file for example)
 */
void config_fix()
{
	int i;
	for (i=0; i<keydefs.size(); i++)
	{
		if (keydefs[i].name=="")
			keydefs[i].name=string("Keyboard")+toString_int(i);
	}
}

/**
 * Check for the default.ttf file in the given directory
 * if it exists set the global datadir variable to the given directory
 * if the datadir variable was already set to something different than
 * an empty string bail out without doing anything
 *
 * @param td the directory to look into
 */
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
	// set paths
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



