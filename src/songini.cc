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

#include "songini.h"

#include "configuration.h"
#include "message.h"
#include "verbosity.h"

#define tsiMAstring(c) if (tsimatch(l,""#c)) {c=l; DBG(SONGINI,"Read: "#c"=%s\n", c); continue; } else do {;} while (0);
#define tsiMAint(c) if (tsimatch(l,""#c)) {c=atoi(l); DBG(SONGINI,"Read int:"#c"=%d\n", c); continue; } else do {;} while(0);

int tSongini::load(string filename)
{
	char buf[16384];
	FILE *f=fopen(filename.c_str(),"r");
	if (!f) return 0;
	while (!feof(f))
	{
		char *l=buf;
		if (NULL==fgets(buf,16383,f)) break;
		buf[16383]=0;
		while (*l==' ' || *l=='t')  l++;
		#define SIPD(a,b) tsiMA##a(b);
		SONGINI_LIST
		#undef SIPD
	}
	fclose(f);
	return 1;
}



