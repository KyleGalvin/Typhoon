namespace SDL_Render{

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

bool initscreen(const int, const int, const int, SDL_Surface**);
bool loadimage(const char*, SDL_Surface**);
void mergesurface(int,int,SDL_Surface**,SDL_Surface**);
void mergesurface(int,int,SDL_Rect*,SDL_Surface**,SDL_Surface**);

//Initializes screen and all SDL subsystems
bool initscreen(const int SCREEN_WIDTH, const int SCREEN_HEIGHT, const int SCREEN_BPP, SDL_Surface **screen)
{
	//initiate all SDL subsystems
	if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
		return false;
	}

	//open program window
	*screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE/* | SDL_FULLSCREEN*/);
	if(*screen == NULL){
		return false;
	}

	//window caption
	SDL_WM_SetCaption("Tankarank!",NULL);

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
			optimizedimage=SDL_DisplayFormat(loadedimage);
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
