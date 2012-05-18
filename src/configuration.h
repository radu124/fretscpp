/**
 * @file configuration.h global variables for program settings that
 * will be loaded and written into the user configuration file
 */
#ifndef __HDR_configuration_h
#define __HDR_configuration_h


#include "includes.h"
#include "player.h"
#include "confrw.h"

/** path to the configuration file */
extern string configFile;
/** path to the file storing the player progress and statistics */
extern string scoresFile;
/** directory where the configuration will be saved,
 * under Linux, it defaults to $HOME/.config/fretscpp */
extern string homeconfdir;
/** directory where the game data is stored, the game will look for
 * the default.ttf font in the following directories and will assign the
 * data dir to the first path found
 *   <homeconfdir>/data
 *   ./data
 *   /usr/share/games/fretscpp/data
 *   /usr/local/share/games/fretscpp/data
 *   /usr/share/games/fretsonfire/data
 */
extern string datadir;

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

/**
 * This is a list of configuration variables, their types and default values
 * this list is used 4 times
 *   1) declaring the variable extern
 *   2) declaring the variable
 *   3) reading the variable from the configuration file
 *   4) writing the variable to the configuration file
 * each time, CONFIGITEM is substituted with a different macro
 */
#define CONFIGLIST \
CONFIGITEM(GLfloat,tolerance_early,120)         /* gameplay - toler. early*/  \
CONFIGITEM(GLfloat,tolerance_late,120)          /* gameplay - toler. late */  \
CONFIGITEM(int,tappablemode,0)                  /* gameplay - tappable    */  \
CONFIGITEM(int,strictkeys,0)                    /* gameplay - strict      */  \
CONFIGITEM(int,miss_loudness,200)               /* volume - miss          */  \
CONFIGITEM(int,guitar_volume,100)               /* volume - good note     */  \
CONFIGITEM(int,guitar_off_volume,20)            /* volume - bad note      */  \
CONFIGITEM(int,conf_stretch,0)                  /* stretch scene 4:3      */  \
CONFIGITEM(int,video_glflush,0)                 /* video use glFlush()    */  \
CONFIGITEM(int,video_glfinish,0)                /* video use glFinish()   */  \
CONFIGITEM(int,video_vsync,0)                   /* video vsync            */  \
CONFIGITEM(int,video_limiter,0)                 /* video fps limit enable */  \
CONFIGITEM(GLfloat,video_fpslim,60)             /* video fps limit        */  \
CONFIGITEM(GLfloat,default_neck_velocity,1)     /* default neck velocity  */  \
CONFIGITEM(int,video_dofullscreen,0)            /* video full screen      */  \
CONFIGITEM(int,video_overrideres,0)             /* video not desktop res  */  \
CONFIGITEM(int,scr_width,800)                   /* screen/window horiz res*/  \
CONFIGITEM(int,scr_height,600)                  /* screen/window vert res */  \
CONFIGITEM(string,themeStage,"rf-mod")          /* theme - stage (play)   */  \
CONFIGITEM(string,themeMainm,"default")         /* theme - main menu      */  \
CONFIGITEM(string,themeScore,"default")         /* theme - score scene    */  \
CONFIGITEM(string,themeSlist,"default")         /* theme - song list      */  \
CONFIGITEM(string,themeSopts,"default")         /* theme - song options   */  \
CONFIGITEM(string,themeSettm,"default")         /* theme - settings scene */  \
CONFIGITEM(string,themeNeckname,"Default")      /* theme - neck name      */  \
CONFIGITEM(int,themeNecktransparency,0)         /* theme - neck alpha     */  \
CONFIGITEM(GLfloat,neckpos_extend,0)            /* neck - extend          */  \
CONFIGITEM(GLfloat,boardlen,4)                  /* neck - boardlen        */  \
CONFIGITEM(GLfloat,boardline,3)                 /* neck - boardline       */  \
CONFIGITEM(GLfloat,necktop,2)                   /* neck - global top      */  \
CONFIGARRAY(GLfloat,neckpos,.xdisp,.xdisp)      /* neck - horizontal pos  */  \
CONFIGARRAY(GLfloat,neckpos,.ydisp,.ydisp)      /* neck - vertical pos    */  \
CONFIGARRAY(GLfloat,neckpos,.scale,.scale)      /* neck - scale x-y       */  \
CONFIGARRAY(GLfloat,neckpos,.stretch,.stretch)  /* neck - stretch x       */  \
CONFIGARRAY(GLfloat,neckpos,.skew,.skew)        /* neck - make slanted    */  \
CONFIGARRAY(int,neckpos,.statpos,.statpos)      /* status on left/right   */  \
CONFIGARRAY(int,keydefs,.key[0],.fret1)         /* keymap fret1           */  \
CONFIGARRAY(int,keydefs,.key[1],.fret2)         /* keymap fret2           */  \
CONFIGARRAY(int,keydefs,.key[2],.fret3)         /* keymap fret3           */  \
CONFIGARRAY(int,keydefs,.key[3],.fret4)         /* keymap fret4           */  \
CONFIGARRAY(int,keydefs,.key[4],.fret5)         /* keymap fret5           */  \
CONFIGARRAY(int,keydefs,.key[5],.pick)          /* keymap pick            */  \
CONFIGARRAY(int,keydefs,.key[6],.altpick)       /* keymap altpick         */  \
CONFIGARRAY(int,keydefs,.key[7],.whammy)        /* keymap whammy          */  \
CONFIGARRAY(int,keydefs,.key[8],.power)         /* keymap power           */  \
CONFIGARRAY(string,keydefs,.name,.name)         /* keymap name            */  \


/**
 * Substitution of CONFIGITEM that will declare the variable b of type a
 * with default value c
 */
#define CI_DECLARE(a,b,c) a b=c;

/**
 * Substitution of CONFIGITEM that will declare the variable b extern
 */
#define CI_DECLARE_EXTERN(a,b,c) extern a b;

/**
 * Substitution of CONFIGITEM that will produce configuration file output
 * for the variable b
 */
#define CI_PRINT(a,b,c)         \
{                               \
	fprintf(fou,""#b" = ");     \
	CONFWRITE_##a(b);           \
	fprintf(fou,"\n");          \
}

/**
 * Substitution of CONFIGARRAY that will produce configuration file output
 * for the array b with elements of type a with field name d in the config
 * file and field name c in the source code
 * Note: arrays need to be declared manually
 */
#define CIA_PRINT(a,b,c,d)                      \
{                                               \
	int it;                                     \
	for (it=0; it<b.size(); it++)               \
	{                                           \
		fprintf(fou,""#b"[%d]"#d" = ",it);      \
		CONFWRITE_##a(b[it]c);                  \
		fprintf(fou,"\n");                      \
	}                                           \
}

/**
 * Substitution of CONFIGITEM that will produce matching and reading
 * from the configuration file
 *
 * Check if the (char *) string p (in current scope) matches the name of b
 * and if it does read variable b of type a from p
 */
#define CI_READ(a,b,c)                          \
{                                               \
	if (tsimatch(p,""#b))                       \
	{                                           \
		CONFREAD_##a(p,b);                      \
	}                                           \
}

/**
 * Substitution of CONFIGARRAY that will produce matching and reading
 * of a configuration array item from the configuration file
 *
 * Check in the (char *) string p (in current scope) whether the name of
 * b and suffix d matches and read the variable if it does
 * resize the array if necessary
 */
#define CIA_READ(a,b,c,d)                        \
{                                                \
	int idx;                                     \
	if (tsamatch(p,""#b,""#d,idx))               \
	{\
		assume(idx<1000,"Bad array index>1000"); \
		if (idx>=b.size()) b.resize(idx+1); \
		CONFREAD_##a(p,b[idx]c);                 \
	}\
}

/*************************************************************
 * Automatic generation of config variable extern declarations
 */
#define CONFIGITEM CI_DECLARE_EXTERN
#define CONFIGARRAY(a,b,c,d)
CONFIGLIST
#undef CONFIGARRAY
#undef CONFIGITEM

/**
 * Array containing a list of keys assigned to particular actions
 *   key[0] = fret1
 *   key[1] = fret2
 *   key[2] = fret3
 *   key[3] = fret4
 *   key[4] = fret5
 *   key[5] = pick
 *   key[6] = altpick
 *   key[7] = whammy
 *   key[8] = power
 * name - the name of this key configuration
 */
struct tKeydefset
{
	int key[9];
	string name;
};

/**
 * a vector of key configurations from which the user can select
 */
extern vector<tKeydefset> keydefs;

/**
 * the position of a guitar neck in the main game screen
 */
struct tNeckpos
{
	/**
	 *  horizontal displacement
	 */
	GLfloat xdisp;
	/**
	 * vertical displacement
	 */
	GLfloat ydisp;
	/**
	 * scale factor, how much the neck is scaled in both x and y directions
	 */
	GLfloat scale;
	/**
	 * stretch factor, stretch in the x, but not the y direction
	 */
	GLfloat stretch;
	/**
	 * left-right geometric (and visual) correction
	 * positive values will shift the top of the neck to the right
	 * and the bottom to the left while negative values will have the opposite effect
	 */
	GLfloat skew;
	/**
	 * whether the status should be on the left or on the right
	 * 0 = left
	 * 1 = right
	 */
	int statpos;
};

/**
 *  bit mask describing the last selected input devices
 */
extern long keydefselector[MAX_PLAYERS];

const int NECKPOS_COUNT=MAX_PLAYERS*(MAX_PLAYERS+1)/2+1;

/**
 * vector of neck positions for all player-modes
 * [0] is for one player
 * [1][2] are for the two-player mode
 * [3][4][5] are for the three-player mode
 * [6][7][8][9] are for the four-player mode
 * [10] is for the theme scene
 */
extern vector<tNeckpos> neckpos;

/**
 * Default neck positions to initialize neckpos
 * when no configuration file is available
 */
extern tNeckpos defneckpos[11];

/**
 * difficulty names
 */
extern const char *difficultynames[4];

/**
 * vector that will store the unknown configuration variables
 * so that they are not lost when rewriting the configuration file
 */
extern vector<string> config_unknown;

/**
 * initialize paths (in particular homeconfdir, datadir, configFile
 * and scoresFile) and read configuration file
 *
 */
void init_config();

/**
 * Read the configuration variables from the configuration file
 * indicated by configFile
 */
int config_read();

/**
 * write the configuration variables to the configuration file
 */
void config_write();

/**
 * Match a string read from the configuration file against the name of
 * a configuration variable, a successful match is considered if the string
 * is of the form:
 * variablename<space?>=
 * @param s (char *) input string, passed as reference, on a successful
 *   match, s is increased by the length of the variable name,
 *   additional space and 1 for the equal sign
 * @param v the variable
 * @return 1 on a match 0 otherwise
 */
int tsimatch(char *&s, const char *v);

/**
 * Match a string read from the configuration file against the name of
 * a configuration item in an array
 * a successful match is considered if the string is of the form:
 * prefix[idx]suffix<space?>=
 * @param s (char *) input string, passed as reference, on a successful
 *   match, s is increased by the length of the variable name,
 *   additional space and 1 for the equal sign
 * @param prefix the name of the array
 * @param suffix an optional member field, useful for example when the
 *   array elements are structs
 * @param idx output variable that will store the array index on a
 *   successful match
 * @return 1 on a match 0 otherwise
 */
int tsamatch(char *&s, const char *prefix, const char *suffix, int &idx);


#endif
