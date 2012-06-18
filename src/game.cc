/*******************************************************************
(C) 2010 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

#include "includes.h"
#include "utils.h"
#include "font.h"
#include "sprites.h"
#include "texManager.h"
#include "globals.h"
#include "scnMainMenu.h"
#include "scnGuitar.h"
#include "scnKeydef.h"
#include "scene.h"
#include "audio.h"
#include "midiparser.h"
#include "verbosity.h"
#include "joystick.h"
#include "video.h"
#include "audiosong.h"
#include "configuration.h"
#include "confrw.h"
#include "sfxfof.h"
#include "score.h"
#include "keyboard.h"
#include "stage.h"
#include "playGfx.h"

void drawglstuff()
{
	//DBG(GL,"%d\n" &((int) scn.time));
	renderscene();
	audio_yield();
}

int main( int argc, char* argv[] )
{
	init_config();
	assume(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO), "Unable to initialize SDL: %s\n");
	findvideomodes();
	if (!video_overrideres) checkFullscreenRes();
	// SDL_GetError());
	init_fonts();
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	SDL_WM_SetCaption("Frets-on-fire-CPP", "SDL");
	/* create window */
	scrsurface = SDL_SetVideoMode(
		scr_width,scr_height,
		 0, SDL_OPENGL | SDL_RESIZABLE |
		(video_dofullscreen?SDL_FULLSCREEN:0) );
	fullscreenmode = video_dofullscreen;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
	INFO(SPRITES,"Maximum texture size:%d\n", maxTexSize);
	init_joysticks();
	glEnable( GL_TEXTURE_2D );
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glAlphaFunc(GL_GREATER,0.1);
	//glEnable(GL_ALPHA_TEST);

	//glBlendFunc (GL_BLEND_DST_ALPHA ,GL_SRC_ALPHA);
	deffont.open(datadir+"/default.ttf");
	init_sprites();
	stagePlay=new Stage();
	st_mainm =new Stage();
	st_sopts =new Stage();
	st_slist =new Stage();
	st_score =new Stage();
	st_settm =new Stage();
	playgfx  =new playGfx();

	stagePlay->load(datadir+"/stages/"+themeStage);
	st_mainm ->load(datadir+"/st_mainm/"+themeMainm);
	st_sopts ->load(datadir+"/st_sopts/"+themeSopts);
	st_slist ->load(datadir+"/st_slist/"+themeSlist);
	st_score ->load(datadir+"/st_score/"+themeScore);
	st_settm ->load(datadir+"/st_settm/"+themeScore);
	playgfx  ->load(datadir+"/playgfx/"+themePlay);

	init_audio();
	load_all_sfx();
	song_start(datadir+"/menu.ogg");
	audio_yield();
	if (0) {
		int q;
		for (q=0; q<90000; q++) DBG(AUDIO,"%09d %d %d %d %d %d %d\n", q
			,songfiles[0].buffer[0][q]
			,songfiles[0].buffer[1][q]
			,songfiles[1].buffer[0][q]
			,songfiles[1].buffer[1][q]
			,songfiles[2].buffer[0][q]
			,songfiles[2].buffer[1][q]
					      );
	}
	MainMenu.enter(); // also does init
	scores.init();
	glViewport( 0, 0, scr_width, scr_height );
	glClear( GL_COLOR_BUFFER_BIT );
	scene_setOrtho();
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	/* load bitmap to temp surface */
	//SDL_Surface* temp = SDL_LoadBMP("sdl_logo.bmp");
	/* convert bitmap to display format */
	//SDL_Surface* bg = SDL_DisplayFormat(temp);
	/* free the temp surface */
	//SDL_FreeSurface(temp);

	SDL_Event event;
	gameover = 0;

	/* message pump */
	while (!gameover)
	{
		/* look for an event */
		while (SDL_PollEvent(&event)) {
			/* an event was found */
			if (event.type==SDL_QUIT)
				gameover=1;
			else if (event.type==SDL_VIDEORESIZE)
			{
				doResize(event.resize.w,event.resize.h);
			}
			else
				translateandhandleevent(event);

			switch (event.type) {
				/* close button clicked */
				case SDL_QUIT:
					gameover = 1;
					break;
			}
		}

		/* draw the background */
		//SDL_BlitSurface(bg, NULL, screen, NULL);
		/* update the screen */
		//SDL_UpdateRect(screen, 0, 0, 0, 0);
		drawglstuff();
		/*verify that we didn't lose the context*/
		//glIsTexture(tex);
		if (video_glflush) glFlush();
		if (video_glfinish) glFinish();
		SDL_GL_SwapBuffers();
		texStepAVIs();
	}

	/* free the background surface */
	//SDL_FreeSurface(bg);

	/* cleanup SDL */
	config_write();
	done_joysticks();
	SDL_Quit();
	//DBG(GAME,"CBK=%d F:%d Fo:%d chn:%d buf%d\n",
	//	callbacklen,desired.format, obtained.format, obtained.channels, obtained.samples);
	return 0;
}




