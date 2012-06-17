#include "geotypes.hpp"
#include "SDL/SDL.h"
#include "render.hpp"
#include "timer.hpp"
#include "hgrid.hpp"
#include "objectstructs.hpp"
#include <iostream>

using namespace std;

int main(){
	const int screen_w = 1024;
	const int screen_h = 768;
	const int screen_bpp = 32;
	const int fps = 60;

	SDL_Surface *screen;
	SDL_Surface *background;

	SDL_Event event;
	
	Timer timer;
	bool quit = false;

	SDL_Render::initscreen(screen_w,screen_h,screen_bpp,&screen);
	SDL_Render::loadimage("testbackimage.jpg",&background);

	//game loop
	while(!quit){
		timer.start();

		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				quit=true;
			}

			//update mouse info once we have pointer set up

		}

		SDL_Render::mergesurface(0,0,&background,&screen);

		if(SDL_Flip(screen)==-1){
			cout<<"Screen failed to flip\n";
		}

		//throttle framerate
		while(timer.get_ticks()<1000/fps){}
	}

	return 0;
}
