#ifndef __HDR_keyboard_h
#define __HDR_keyboard_h

#include "includes.h"

void guitarkey(int ev, int key, int other=0);
string keyname(int id);
void translateandhandleevent(SDL_Event &event);

/**
 * Allow navigating the menus using the selected controller
 * translate the event into Arrow keys and RETURN
 */
void menukey(SDL_Event &event, int flags=0);


#endif
