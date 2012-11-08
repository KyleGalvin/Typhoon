
#include "SDL/SDL.h"
#include "./src/render.hpp"

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

	init(sdl.screen_w,sdl.screen_h);
	SpriteData sprites;
	
	SDL_Render::initscreen(sdl.screen_w,sdl.screen_h,sdl.screen_bpp,&sdl.screen);
	SDL_Render::loadimage("./sprites/testbackimage.png",&sprites.background);
	SDL_Render::loadimage("./sprites/marker.png",&sprites.somMarkerStamp);
	SDL_Render::loadimage("./sprites/trainmarker.png",&sprites.trainMarkerStamp);
	SDL_Render::loadimage("./sprites/nearestmarker.png",&sprites.nearestMarkerStamp);

	return 0;
}

