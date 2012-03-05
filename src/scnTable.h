#ifndef __HDR_scnTable_h
#define __HDR_scnTable_h


#include "includes.h"
#include "verbosity.h"
#include "scene.h"

class tScnTable:public sceneBase
{
public:
	tScnTable();
	GLfloat fadespeed() { return 100; }
	void render();
	void rendertext();
	void enter();
	void handleevent(SDL_Event &a);
	virtual void itemClicked(int i, int j=0);
	virtual void itemClickedSep(int k, int i, int j, int &o) {;}
	void additem(string s);
	void selfix();
	void optinit();
	void newcol();
	void cleancol();
	void dupcol();
	void makeempty();
	char getMarker(int i, int j); // first letter of title
	int groupStart(int i,int g);
	void touch(int i, int j);
	
	/**
	 * remember what the last command was so that we can skip in
	 * the direction of the selection
	 */
	int lastlr; // 0=left
	int lastud; // 0=up

	/** table of entries[column][row]
	 */
	vvstring entries;

	/**
	 * per item flags
	 * 1 = selected
	 * 2 = invisible
	 * 4 = disabled
	 */
	vvint opt; // per item flags
	
	int selx, sely;
	int baseh;
	GLfloat colsize;
	GLfloat xpos;
	int colsvisible;
	int fixedcols;
	int mustregenopt;
	
};


#endif
