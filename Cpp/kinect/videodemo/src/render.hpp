#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

namespace SDL_Render{

bool initscreen(const int, const int, const int, SDL_Surface**, const char *title);
bool loadimage(const char*, SDL_Surface**);
void mergesurface(int,int,SDL_Surface**,SDL_Surface**);
void mergesurface(int,int,SDL_Rect*,SDL_Surface**,SDL_Surface**);

struct SDLContext{
	static const int screen_w=640;
	static const int screen_h=480;
	static const int screen_bpp=32;
	static const int fps=60;
	SDL_Surface *screen;
	SDL_Event event;
	bool quit;
};

//Initializes screen and all SDL subsystems
bool initscreen(SDLContext *sdl, const char* title)
{
	sdl->quit = false;
	
	//initiate all SDL subsystems
	if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
		return false;
	}

	//open program window
	sdl->screen = SDL_SetVideoMode(sdl->screen_w,sdl->screen_h,sdl->screen_bpp,SDL_SWSURFACE/* | SDL_FULLSCREEN*/);
	if(sdl->screen == NULL){
		return false;
	}

	//window caption
	SDL_WM_SetCaption(title,NULL);

	return true;
}

//Takes image file and loads to variable of type SDL_Surface*
bool loadimage(const char* imagename,SDL_Surface **loadto)
{

	SDL_Surface *loadedimage=NULL;
	SDL_Surface *optimizedimage=NULL;

	loadedimage= IMG_Load(imagename);
		if(!loadedimage){
			return false;
		}else{
			optimizedimage=SDL_DisplayFormatAlpha(loadedimage);
		}

			SDL_FreeSurface(loadedimage);
		
		if(optimizedimage != NULL){
			//Hexidecimal colour code to set to sprite transparency
			Uint32 colorkey=SDL_MapRGB(optimizedimage->format,0,0xFF,0xFF);
			SDL_SetColorKey(optimizedimage,SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
		}else{
			return false;
		}

	*loadto=optimizedimage;
	return true;
			
}

//Merges surface 'top' onto surface 'base' with offsets x and y
void mergesurface(int x,int y,SDL_Surface** top,SDL_Surface** base)
{
	SDL_Rect rectangle;
	rectangle.x=x;
	rectangle.y=y;

	SDL_BlitSurface(*top,NULL,*base,&rectangle);
}

void mergesurface(int x,int y,SDL_Rect* clip,SDL_Surface** top,SDL_Surface** base)
{
	SDL_Rect rectangle;
	rectangle.x=x;
	rectangle.y=y;

	SDL_BlitSurface(*top,clip,*base,&rectangle);
}

}
