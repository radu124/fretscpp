#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <queue>

using namespace std;

char *hfn;
char *hdrmarker;
char *hdrshortfn;
const char *cpxfn;

FILE *fin;
FILE *fou;
FILE *foh;

string expandpath(string s);

int crtfile;
int verbose=0;
int showlines=1;
int inheader=0;
const char *exename="cpxprep";

char *namespacename(char *filename);

void beginheader();

void assume(int b,const char *c,const char *extra="")
{
    if (b) return;
    fprintf(stderr,"Error: %s.\nErrno:%x\n%s\n",c,errno,extra);
    exit(1);
}

string expandpath(string s)
{
	int i;
	for (i=0; i<s.size(); i++) if (s[i]=='.') s[i]='/';
	return s;
}

int strxcmp(const char *a, string txt, int pos)
{
	char c;
	int r;
	int len=strlen(a);
	assume(pos<=txt.size(),"strxcmp looking beyond end-of-string");
	r=strncmp(a,txt.c_str()+pos,len);
	if (r) return r;
	//    printf("b[len]-%c-%i-",b[len],b[len]);
	if (len+pos==txt.size()) return 0;
	c=txt[len+pos];
	if (c!=' ' && c!='\t' && c!=0 && c!=0 && c!=10 && c!=13 && c!=9 && c!=8) return -1;
	return 0;
}

int isfilenamechar(int v)
{
	return isalnum(v)
		|| v=='.' || v=='_' || v=='-'
			;
}

string readLine(FILE *f)
{
	string res="";
	int l,eoln=0;
	char txt[4096];
	char *unused;
	while (1)
	{
		txt[0]=0;
		unused=fgets(txt,4095,f);
		txt[4096]=0;
		l=strlen(txt);
		if (l==0) return res;
		if (txt[l-1]==10 || txt[l-1]==13)
		{
			eoln=1;
			if (l>1 && txt[l-2]==13) txt[l-2]=0;
			txt[l-1]=0;
		}
		res+=txt;
		return res;
	}
}

void process()
{
	int counter=0;
	int pos,pos2;
	string txt,un;

	while (!feof(fin))
	{
		txt=readLine(fin);
		counter++;
		pos=0;
		while (txt[pos]==' ' || txt[pos]=='\t') pos++;
		if (!strxcmp("implementation",txt,pos))
		{
			fprintf(stderr,"  * encountered implementation, deferring processing\n");
			inheader=0;
			if (foh) fprintf(fou,"#include \"%s\"\n",hdrshortfn);
			continue;
		}
		if (!strxcmp("interface",txt,pos))
		{
			inheader=1;
			beginheader();
			continue;
		}
		if (!strxcmp("uses",txt,pos))
		{
			pos+=5;
			while (1)
			{
				// we allow multiple included files in an uses clause
				while (pos<txt.size() && isblank(txt[pos])) pos++;
				//skip leading white spaces
				if (pos>=txt.size())
				{
					fprintf(stderr,"%s:%d cpxprep error\n",cpxfn,counter);
					fprintf(stderr,"*** End of line encountered in uses clause, expecting semicolon. ***");
					exit(1);
				}
				if (txt[pos]==';') break;
				if (txt[pos]==',') { pos++; continue; }
				pos2=pos;
				while (pos<txt.size() && isfilenamechar(txt[pos])) pos++;
				un=txt.substr(pos2,pos-pos2);
				fprintf(inheader?foh:fou,"#include \"%s.h\"\n",expandpath(un).c_str());
			}
			continue;
		}
		if (inheader)
		{
			fprintf(foh,"%s\n",txt.c_str());
		}
		else
		{
			fprintf(fou,"%s\n",txt.c_str());
		}
	}
	fprintf(fou,"\n\n");
}

void beginheader()
{
	if (foh) return;
	foh=fopen(hfn,"w");
	assume(foh!=NULL,"Could not open .h output");
	fprintf(foh,
"#ifndef __HDR_%s\n"
"#define __HDR_%s\n\n"
,hdrmarker,hdrmarker);
}

void endheader()
{
	if (!foh) return;
	fprintf(foh,"\n#endif\n");
}

int main(int argc, char *argv[])
{
	int i;
	string inputname;
	assume(argc==3,"usage: cpx2cc <input.cpx> <output.cc>");
	fin=fopen(argv[1],"r");
	assume(fin!=NULL,"Could not read input file");
	fou=fopen(argv[2],"w");
	hfn=strdup(argv[2]);
	int hfl=strlen(hfn)-1;
	while (hfl>0 && hfn[hfl]!='.') hfl--;
	hfn[hfl+1]='h';
	hfn[hfl+2]=0;
	cpxfn=argv[1];
	hdrshortfn=hfn;
	for (i=0; hfn[i]; i++) if (hfn[i]=='/') hdrshortfn=hfn+i+1;
	hdrmarker=strdup(hfn);
	for (i=0; hdrmarker[i]; i++) if (!isalnum(hdrmarker[i])) hdrmarker[i]='_';
	assume(fou!=NULL,"Could not open .cc output");
	process();
	endheader();
	return 0;
}
