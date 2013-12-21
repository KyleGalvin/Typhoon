
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "libfreenect.h"

#include "SDL/SDL.h"
#include "render.hpp"

#include "kinect.hpp"

// Initialize new window and get SDL context struct to control it.
SDL_Render::SDLContext* SDLGetContext(){
	//sdl screen info and sprite data
	SDL_Render::SDLContext* sdl = new SDL_Render::SDLContext();
	if(!SDL_Render::initscreen(sdl,"KinectVideoDemo")){
		std::cout<<"SDL context failed to initialize\n";
		exit(1);
	}
	SDL_ShowCursor(0);
	return sdl;
};

MyFreenectDevice* KinectGetContext(){

	// pointer to the freenect context
	freenect_context *ctx;

	if (freenect_init(&ctx, NULL) < 0)
	{
		std::cout<<"freenect_init() failed\n";
		exit(EXIT_FAILURE);
	}

	freenect_set_log_level(ctx, FREENECT_LOG_SPEW);
	MyFreenectDevice *myKinect = new MyFreenectDevice(ctx,0);
	return myKinect;

}

int main(int argc, char *argv[]){

	// Initialize new window and get SDL context struct to control it.
	SDL_Render::SDLContext* sdl = SDLGetContext();

	// Initialize first kinect device and prep video for reading.
	MyFreenectDevice *myKinect= KinectGetContext();	
	myKinect->startVideo();
	myKinect->setVideoFormat(FREENECT_VIDEO_RGB);
	
	// Custom mapping between kinect video format and SDL surface format
	std::vector<unsigned uint8_t> myRGBFrame(640*480*3);
	SDL_Rect* pixelLocation = new SDL_Rect();
	pixelLocation->h=10;
	pixelLocation->w=10;
	pixelLocation->x= 50;
	pixelLocation->y= 50;
				SDL_FillRect(sdl->screen,pixelLocation,0xFFFFFFFF);
	pixelLocation->h=1;
	pixelLocation->w=1;
	uint32_t pixelFormatSDL;
	uint8_t pixelFormatKinR;
	uint8_t pixelFormatKinG;
	uint8_t pixelFormatKinB;

	while (sdl->quit == false){
		// Custom mapping between kinect video format and SDL surface format
		myKinect->updateState();
		bool result = myKinect->getRGB(myRGBFrame);
		//std::cout<<"array size: "<<myRGBFrame.size()<<"\n";
		std::cout<<"result: "<<result<<"\n";
		for(unsigned int i=0; i<myRGBFrame.size(); i++){
			//std::cout<<"Raw Pixel Data: "<<myRGBFrame[i]<<"\n";
			switch(i%3){
			case 0:
				pixelFormatKinR = myRGBFrame[i];
			break;
			case 1:
				pixelFormatKinG = myRGBFrame[i];
			break;
			case 2:
				pixelFormatKinB = myRGBFrame[i];
				//we aren't using the alpha channel, but we can use this case to do the math between pixels
				pixelLocation->x= (i % 640);//divide by the 4 channels per pixel we are currently iterating over
				pixelLocation->y=(int) i/(640*3);
				pixelFormatSDL = (pixelFormatKinR >>24) | (pixelFormatKinG >> 16) | (pixelFormatKinB >>8 | 0xFF);
				SDL_FillRect(sdl->screen,pixelLocation,pixelFormatSDL);
			break;
			}
		}
		std::cout<<"\n end of frame\n";
		SDL_Flip(sdl->screen);
	}
}
