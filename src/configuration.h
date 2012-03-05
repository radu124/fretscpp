#ifndef __HDR_configuration_h
#define __HDR_configuration_h


#include <string>
#include "player.h"
#include "confrw.h"

using namespace std;

extern string configFile;
extern string scoresFile;
extern string homeconfdir;
extern string datadir;
// just decided namespaces are evil
// namespaces allow you to have two things with the same name
// why would you want that, just to create confusion?
// namespace config

// for selection of the neck we have the following variables
// the filename of the neck
extern string themeNeckfilename;
// the index in the list of necks
extern int themeNeckidx;
// the config name of the neck: themeNeckname
extern vstring themeNecknames;
extern vstring themeNeckfilenames;
extern vstring themeStages;
extern vstring themesMainm;
extern vstring themesScore;
extern vstring themesSlist;
extern vstring themesSopts;
extern vstring themesSettm;

#define CONFIGLIST \
CONFIGITEM(GLfloat,neckpos_extend,0) \
CONFIGITEM(GLfloat,tolerance_early,120) \
CONFIGITEM(GLfloat,tolerance_late,120) \
CONFIGITEM(int,tappablemode,0) \
CONFIGITEM(int,strictkeys,0) \
CONFIGITEM(int,miss_loudness,200) \
CONFIGITEM(int,guitar_volume,100) \
CONFIGITEM(int,guitar_off_volume,20) \
CONFIGITEM(GLfloat,boardlen,4) \
CONFIGITEM(GLfloat,boardline,3) \
CONFIGITEM(GLfloat,necktop,2) \
CONFIGITEM(int,video_glflush,0) \
CONFIGITEM(int,video_glfinish,0) \
CONFIGITEM(int,video_vsync,0) \
CONFIGITEM(int,video_limiter,0) \
CONFIGITEM(GLfloat,video_fpslim,60) \
CONFIGITEM(int,video_dofullscreen,0) \
CONFIGITEM(int,video_overrideres,0) \
CONFIGITEM(int,scr_width,800) \
CONFIGITEM(int,scr_height,600) \
CONFIGITEM(string,themeStage,"rf-mod") \
CONFIGITEM(string,themeMainm,"default") \
CONFIGITEM(string,themeScore,"default") \
CONFIGITEM(string,themeSlist,"default") \
CONFIGITEM(string,themeSopts,"default") \
CONFIGITEM(string,themeSettm,"default") \
CONFIGITEM(string,themeNeckname,"Default") \
CONFIGITEM(int,themeNecktransparency,0) \
CONFIGARRAY(GLfloat,neckpos,.xdisp,.xdisp) \
CONFIGARRAY(GLfloat,neckpos,.ydisp,.ydisp) \
CONFIGARRAY(GLfloat,neckpos,.scale,.scale) \
CONFIGARRAY(GLfloat,neckpos,.stretch,.stretch) \
CONFIGARRAY(GLfloat,neckpos,.skew,.skew) \
CONFIGARRAY(int,neckpos,.statpos,.statpos) \
CONFIGARRAY(int,keydefs,.key[0],.fret1) \
CONFIGARRAY(int,keydefs,.key[1],.fret2) \
CONFIGARRAY(int,keydefs,.key[2],.fret3) \
CONFIGARRAY(int,keydefs,.key[3],.fret4) \
CONFIGARRAY(int,keydefs,.key[4],.fret5) \
CONFIGARRAY(int,keydefs,.key[5],.pick) \
CONFIGARRAY(int,keydefs,.key[6],.altpick) \
CONFIGARRAY(int,keydefs,.key[7],.whammy) \
CONFIGARRAY(int,keydefs,.key[8],.power) \
CONFIGARRAY(string,keyconfname,,) \


#define CI_DECLARE_EXTERN(a,b,c) extern a b;
#define CI_DECLARE(a,b,c) a b=c;
#define CI_PRINT(a,b,c) { fprintf(fou,""#b" = "); CONFWRITE_##a(b); fprintf(fou,"\n"); }
#define CIA_PRINT(a,b,c,d) { int it; for (it=0; it<b.size(); it++) { fprintf(fou,""#b"[%d]"#d" = ",it); CONFWRITE_##a(b[it]c); fprintf(fou,"\n");} }

#define CI_READ(a,b,c) { if (tsimatch(p,""#b)) { confread_##a(p,b); } }
#define CIA_READ(a,b,c,d) { int idx; if (tsamatch(p,""#b,""#d,idx)) \
	{\
		assume(idx<1000,"Bad array index>1000"); \
		if (idx>=b.size()) b.resize(idx+1); \
		confread_##a(p,b[idx]c); \
	}\
}

/*******************************************************
 * Automatic generation of config variable declarations
 */
#define CONFIGITEM CI_DECLARE_EXTERN
#define CONFIGARRAY(a,b,c,d)
CONFIGLIST
#undef CONFIGARRAY
#undef CONFIGITEM


struct tKeydefset
{
	int key[9];
};

extern vector<tKeydefset> keydefs;
extern vstring keyconfname;

struct tNeckpos
{
	GLfloat xdisp,ydisp;
	GLfloat scale,stretch;
	GLfloat skew;
	int statpos;
//	tNeckpos():xdisp(0),ydisp(0),scale(1),stretch(1),skew(0),statpos(0) {;}
};

// bit mask describing the last selected input devices
//we don't expect more than 8 players
extern long keydefselector[8];
extern void init_config();

extern int conf_stretch;
const int NECKPOS_COUNT=MAX_PLAYERS*(MAX_PLAYERS+1)/2+1;

extern vector<tNeckpos> neckpos;
extern vector<string> config_unknown;

extern tNeckpos defneckpos[11];

extern const char *difficultynames[4];

/***********************************************
 * Match configuration string against pattern
 * and increment pointer if match occurred
 */
int tsimatch(char *&s, const char *v);
int tsamatch(char *&s, const char *prefix, const char *suffix, int &idx);
void config_write();

#endif
