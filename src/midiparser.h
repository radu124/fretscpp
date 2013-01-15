#ifndef __HDR_midiparser_h
#define __HDR_midiparser_h


#include "includes.h"

/**
 * The status of notes at one time in the midi file
 */
struct notestatus
{
	int timestamp;
	int timeticks;
	double tickduration;
	/**
	 * Each character represents a difficulty level (with spares)
	 * levels separated by 12 characters 0-4=easy
	 * O - beginning of a note
	 * - note continues
	 * X - the end of a note
	 * B - both an end and a beginning
	 */
	char val[64]; // " O-XB"
};

/**
 * tempo setting changes
 */
struct tempoMarker
{
	tempoMarker():time(0),bpm(0){;}
	tempoMarker(double t,double b):time(t),bpm(b) {;}
	/**
	 * the time when this becomes active, in ticks
	 */
	double time;   
	double bpm;    /// new BPM
};

enum notestatusflags
{
	/* initialized during track extraction */
	ENS_HASHIT=1,     // whether the note has an 'O' or a 'B'
	ENS_HASLINE=2,    // whether the note is a '-' note
	ENS_ISMULTI=4,    // whether the note has multiple 'O's or 'B's
	/* initialized during player init */
	ENS_TAPPABLE=8,   // whether the note is tappable
	/* updated during gameplay */
	ENS_WELLPLAYED=16,// whether the note was correctly played
	ENS_EARLYRELEASE=32 // whether the player released the fret early
};

/**
 * note status for a single difficulty level
 */
struct notestatusst
{
	int timestamp;
	char val[6];
	int flags;
	/**
	 * check whether the value at index i is a note
	 * @param i note of interest
	 * @return 1 if val[i] is "O" or "B"
	 */
	int noteon(int i);
	/**
	 * @return 1 if this note has a hit (O or B character)
	 */
	int detect_hit();
	/**
	 * @return 1 if this note has a continuous line
	 */
	int detect_line();
	/**
	 * set from note status
	 */
	void from_note_status(notestatus &v, int difficulty);
	string str();
};

typedef vector<notestatus> vnotestatus;
typedef vector<vnotestatus> vvnotestatus;

/**
 * The midi parser is a mess
 * a midi can have multiple instrument tracks which are stored separately
 * all difficulties though corresponding to a single instrument are stored
 * in the same track
 *
 * only when the game is started the "lane" in the instrument track
 * corresponding to the selected difficulty is copied to the player
 *
 * also file operations are midi specific, and in the mean time I
 * invented another way of accessing files for the video (in fileio)
 * TODO
 */
class MidiParser
{
	/// buffer memory for storing the entire midi file
	unsigned char *buffer;
	/// size of the file
	int size;
	/// current parser position in the file
	int pos;
	/// saved parser position in the file
	int savedpos;
	int chunkend,chunklen;
	/// midi signature
	int format;
	/// number of tracks
	int tracks;
	int ticksperbeat;
	int ts_numer, ts_denom, ts_metro, ts_32nds;
	double tickduration;
	/**
	 * bpm from fof, used indiscriminately across tracks, 
	 * latest parsed counts - in Track:update
	 */
	double fofbpm;
	notestatus a;
	vector<notestatus> score;
protected:
	int FAIL;
	/**
	 * look in the buffer for a given signature, if found, increase pos
	 * by the length of the signature
	 * @param str the signature to look for
	 * @return 1 if found, 0 if not found
	 * only used once!
	 */
	int expect(const char *str);
	/**
	 * @param string to look for
	 * @return 1 if the string matches the buffer at pos
	 */
	int isthere(const char *str);
	/**
	 * parse the entire midi file
	 */
	int doparse();
	/**
	 * read a byte from the buffer and increment pos
	 * @return the value that was read
	 */
	unsigned int readbyte();
	/**
	 * read a 16-bit little endian word from the buffer and
	 * @return the value that was read
	 */
	unsigned int readshort();
	/**
	 * read a 32-bit unsigned little endian word from the buffer and
	 * increment pos
	 * @return the value that was read
	 */
	unsigned int readlong();
	/**
	 * read a variable length midi timecode from the buffer and
	 * increment pos
	 * @return the value that was read
	 */
	unsigned int readvarlen();
	/**
	 * create a new note in score and set its timestamp to the timestamp
	 * of the last note plus delta, adjusted to 1s/44k units, if the
	 * last state of the note was "hit" the new state will change to "-"
	 */
	void timeincrement(int delta);
	/**
	 * Recompute timestamps after all tracks are read
	 */
	void recomputetimestamps();
public:
	int lastevent;
	/**
	 * the MD5 hash of the file
	 */
	uint8_t hash[20];
	/**
	 * delay all notes by the given amount
	 */
	void applydelay(int delay);
	/**
	 * return time, corrected by tempo markers if they exist (from FoF)
	 * @param timestamp
	 */
	double abs_time(double ts);
	/**
	 * ticks-to-beats conversion from FoF
	 */
	double ticksToBeats(double ticks, double bpm);
	/**
	 * List of tempo changes
	 */
	vector<tempoMarker> tempoMarkers;
	/**
	 * vector of tracks, which in turn are vectors of notestatus
	 */
	vvnotestatus trk_notes;
	/**
	 * vector of difficulty masks
	 * the last 4 bits in the mask indicate whether difficulty
	 * corresponding to the bit number (bit 0,1,2,3) is available or not
	 */
	vector<int> trk_difficulties;
	/**
	 * vector of instrument names (not always accurate)
	 */
	vector<string> trk_instrument;

	/**
	 * get a track corresponding to the selected instrument and
	 * difficulty level
	 */
	vector<notestatusst> get_lane(int instrument, int difficulty);
	/**
	 * open and parse midi file
	 */
	int openfile(const char *name);
	/**
	 * debug - dump track contents
	 */
	void debug_dump_track(int track);
};

/**
 * @return 1 if the character is 'O' or 'B'
 */
int ishit(char c);

#endif
