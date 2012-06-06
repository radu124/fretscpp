#ifndef __HDR_fileio_h
#define __HDR_fileio_h


#include "includes.h"

/**
 * List directory contents
 * @param dir the name of the directory
 * @param flags - a combination of the following
 *   1 - also list directories
 *   2 - list only files/directories
 *   4 - exclude files starting with . (dot)
 */
vector<string> listDirContents(string dir, int flags=0);

/**
 * determine if a file exists
 * @return 1 if file exists, 0 otherwise
 */
int fileExists(std::string name);

/**
 * binary file that can be opened for reading
 * supports 64-bit offsets if the underlying OS does
 */
class binaryfile
{
private:
	int fd;
	uint8_t *buffer;
	uint64_t offset;
	uint64_t filesize;
	uint32_t inbuffer;
	uint32_t buffersize;
public:
	/**
	 * constructor
	 */
	binaryfile():
		fd(-1),offset(0),filesize(0),inbuffer(0),buffersize(0) {;}
	~binaryfile();
	/**
	 * return the size of the file
	 */
	uint64_t size();
	/**
	 * return the size of the file
	 */
	uint64_t seek(uint64_t pos);
	/**
	 * read a number of bytes from the file
	 */
	uint64_t readbytes(void *dest, uint64_t len);
	/**
	 * open binary file for reading
	 * for portability because Wdoze makes a difference between binary
	 * files and other
	 */
	int openfile(string path);
	/**
	 * read a signature (a fixed number of characters) from the given file
	 * @param fin the file to read from
	 * @param size the length of the signature
	 * @return the signature
	 */
	string readsig(int size=4);

	/**
	 * read a little-endian 32-bit unsigned integer from the file
	 * @param fin the file to read from
	 * @return the value that was read
	 */
	uint32_t readuint32();

	/**
	 * read a little-endian 16-bit unsigned integer from the file
	 * @param fin the file to read from
	 * @return the value that was read
	 */
	uint16_t readuint16();

};

#endif
