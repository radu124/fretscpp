#ifndef __HDR_utils_h
#define __HDR_utils_h

#include "includes.h"

double gettimenow();

// defined in configuration.h
int tsimatch(char *&s, const char *v);

/**
 * convert the given integer to a string
 * @param i input value
 * @return the same value with plain %d format as a string
 */
string toString_int(int i);

template <typename T>
void dorealloc(T* &adr,int len)
{
	T* res=(T*) realloc(adr,len*sizeof(T));
	if (!res) exit(1);
	adr=res;
}

vector<string> split_string(string s, char c);

#endif
