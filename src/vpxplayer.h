#ifndef __HDR_VPXPLAYER_H
#define __HDR_VPXPLAYER_H

#include "aviparser.h"

class VPXcodecData;

class vpxplayer:public avifile
{
public:
	/**
	 * codec specific structures
	 * allocated with new
	 */
	VPXcodecData *vpxc;
	/**
	 * the width of the texture buffer (power of 2)
	 */
	int glbufwidth;
	/**
	 * the height of the texture buffer (power of 2)
	 */
	int glbufheight;
	/**
	 * buffer to store the decoded image
	 * malloc'd
	 */
	uint8_t *decodeddata;
	/**
	 * currently playing frame
	 */
	int crtframe;
	/**
	 * the last time a frame was decoded
	 */
	uint64_t lastframeadvance;

	vpxplayer():decodeddata(NULL) {;}
	/**
	 * destructor, calls decoder_done
	 */
	~vpxplayer() { decoder_done(); }
	uint8_t frame[1048567];
	/**
	 * @return 1 on success, 0 on failure
	 */
	int decoder_init();
	/**
	 * decode the frame with the given number
	 * @param frameno the frame number
	 * @param pixels the buffer to write into
	 * @param width the width of the buffer
	 * @param height the height of the buffer
	 */
	void decode_frame(int &frameno, uint8_t *pixels, int width, int height);
	/**
	 * deallocate structures
	 */
	void decoder_done();
	/**
	 * read a frame to buffer and increment frameno,
	 * if frameno goes past the end of the movie skip to beginning
	 * if the buffer is not allocated or too small allocate new buffer
	 * @param gltexture texture to assign to
	 * @param time - in microseconds
	 * @param flags
	 * 		1 - immediate
	 */
	int load_next_frame(int gltexture, uint64_t time=0, int flags=0);

};

#endif
