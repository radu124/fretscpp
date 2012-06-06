#ifndef __HDR_aviparser_h
#define __HDR_aviparser_h

#include "includes.h"
#include "message.h"
#include "verbosity.h"
#include "fileio.h"

struct avisubcontainer
{
	string path;
	uint64_t offset;
	string sig;
	string typ;
	uint32_t len;
};

struct AVImainHeader;
struct AVIstreamInfo;

struct idxentry
{
	uint64_t offset;
	uint32_t size;
	uint32_t flags;
};

class avifile
{
public:
	/**
	 * An array of subcontainers
	 * 0 - RIFF/AVI_
	 * 1 - LIST/hdrl
	 * 2 - avih/
	 */
	vector<avisubcontainer> subcs;
	/**
	 * pointer to a structure containing the main AVI header
	 * allocated with new, deallocated in reset
	 */
	AVImainHeader *hdr;
	/**
	 * Pointers to stream info structures
	 * these contain the stream headers and format info
	 * allocated with new, deallocated in reset
	 */
	vector<AVIstreamInfo *> streams;
	/**
	 * The AVI index, the offset inside the file where each frame is found
	 */
	vector<idxentry> vidx;
	/**
	 * the index of the stream containing the video
	 */
	int videostream;
	/**
	 * the file being accessed
	 */
	binaryfile f;
	/**
	 * length of the input file
	 */
	uint64_t flen;
	/**
	 * the offset of the first LIST+movi container
	 * this offset is added to index entries in one of the cases
	 * (it seems we have to guess)
	 */
	uint64_t firstmovi;
	/**
	 * buffer holding compressed video
	 * malloc'd
	 */
	uint8_t *buffer;
	/**
	 * how much memory was allocated for the buffer
	 */
	uint32_t buffer_allocd;
	/**
	 * the width of the first video stream
	 */
	int width;
	/**
	 * the height of the first video stream
	 */
	int height;
	/**
	 * microseconds per frame
	 */
	uint64_t uspf;
	/**
	 * constructor
	 */
	avifile(): hdr(NULL),firstmovi(0),buffer(NULL) {;}
	/**
	 * destructor
	 * calls reset to deallocate pointers
	 */
	~avifile();
	/**
	 * open an avi file and read the index
	 * @return 1 on success, 0 on failure
	 */
	int open(string filename);
	/**
	 * clear the internal tables
	 */
	void reset();
	/**
	 * iterate through containers
	 * @param fin input file
	 * @param path, container path
	 * @param offset, offset inside file
	 * @param len, length
	 */
	int parse_containers(string path, uint64_t offset, uint64_t end);
	/**
	 * read an old-style "idx1" index and put the addresses
	 * of the video frames in vidx
	 */
	void read_old_style_index(int size);
	/**
	 * read a frame to buffer and increment frameno,
	 * if frameno goes past the end of the movie skip to beginning
	 * if the buffer is not allocated or too small allocate new buffer
	 * @param frameno the current frame number
	 * @return the size of the frame
	 */
	int read_frame(int &frameno);
};


#endif
