#include "SDL/SDL.h"
#include "./src/render.hpp"
//OpenCV
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#define X_PIXELS 500
#define Y_PIXELS 500
#define Z_PIXELS 500

struct SDLDrawMetaData{
	static const int screen_w=500;
	static const int screen_h=500;
	static const int screen_bpp=32;
	static const int fps=60;
	SDL_Surface *screen;
};

struct SpriteData{
	SDL_Surface *background;
	SDL_Surface *somMarkerStamp;
	SDL_Surface *trainMarkerStamp;
	SDL_Surface *nearestMarkerStamp;
};

bool init(int x, int y){
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
		return false;
	}

	if ( SDL_SetVideoMode( x,y,32,SDL_SWSURFACE ) == NULL ){
		return false;
	}

	SDL_WM_SetCaption( "SDL", NULL );
	return true; 
}

int main(int count, char** args){
	//sdl screen info and sprite data
	SDLDrawMetaData sdl;
	SpriteData sprites;

	init(sdl.screen_w,sdl.screen_h);
	
	SDL_Event event;
	SDL_ShowCursor(0);
	bool quit = false;



	//game loop
	while(!quit){

		//keyboard/mouse handler
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				quit=true;
			}else if(event.type == SDL_KEYDOWN){
			}else if(event.type == SDL_KEYUP){
			}else if(event.type == SDL_MOUSEMOTION ){
                //		MouseMove(event.motion.xrel, event.motion.yrel, sdl.screen_h/2,sdl.screen_w/2);
			}


		}
	}

	return 0;

}
