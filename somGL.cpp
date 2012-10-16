#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include "./src/objectstructs.hpp"
#include "./src/geotypes.hpp"
#include "./src/hgrid.hpp"
#include "./src/collision.hpp"
#include "./src/camera.hpp"
#include "./src/timer.hpp"
#include "./src/render.hpp"

#define X_PIXELS 500
#define Y_PIXELS 500
#define Z_PIXELS 500

camera *cam;

PtrSetPtrObj MyObjects;
hgrid MyGrid;
PtrObj FocusObj;

float zoom = 45.0;
int mouseX;
int mouseY;

GLuint texture;
GLint nOfColors;
GLenum texture_format;

PtrObj createRandObj(){

	double xratio = 1.0/X_PIXELS;
	double yratio = 1.0/Y_PIXELS;
	double zratio = 1.0/Z_PIXELS;

	int Location[3];
	int Dimensions[3];
	
	for(int i=0;i<3;i++){
		Dimensions[i] = (rand()%99)+1;
		Location[i] = (rand()%(X_PIXELS-Dimensions[i]));
	}
	PtrObj O(new AABB);
	O->Dimensions.push_back(Dimensions[0]*xratio);
	O->Dimensions.push_back(Dimensions[1]*yratio);
	O->Dimensions.push_back(Dimensions[2]*zratio);
	O->Location.push_back(Location[0]*xratio);
	O->Location.push_back(Location[1]*yratio);
	O->Location.push_back(Location[2]*zratio);
	O->debug = false;
	return O;
}

PtrSetPtrObj createObjects(){
	PtrSetPtrObj result(new SetPtrObj);

	srand((unsigned)time(0));
	double xratio = 1.0/X_PIXELS;
	double yratio = 1.0/Y_PIXELS;
	double zratio = 1.0/Z_PIXELS;

	for(int i=0;i<10;i++){
		result->insert(createRandObj());
	}
	FocusObj = createRandObj();
	FocusObj->debug = false;
	result->insert(FocusObj);

	return result;
}

bool drawPoly(){
	glBegin(GL_POLYGON);
	glVertex3d(0.0, 0.0, 0.0); 
	glVertex3d(0.0, 1.0, 0.0);
	glVertex3d(1.0, 1.0, 0.0);
	glVertex3d(1.0, 0.0, 0.0);
	glEnd();

}

void drawAABB(){

	typedef pair<Coordinate,Coordinate> Line;
	typedef vector<Line> LineList;
	typedef std::shared_ptr<LineList> PtrLineList;
	
	SetPtrObj::iterator i = MyObjects->begin();
	PtrLineList Shape;

	//start drawin lines!
	glColor3f(1.0,1.0,1.0);

	while(i !=MyObjects->end()){
		glBindTexture(GL_TEXTURE_2D,1);
		glBegin(GL_QUADS);
		(*i)->Draw3DQuads();	
		++i;
		glEnd();
	}

}

void drawGrid(){

	typedef pair<Coordinate,Coordinate> Line;
	typedef vector<Line> LineList;
	typedef std::shared_ptr<LineList> PtrLineList;
	
	PtrLineList Shape;
	typename hgrid::const_iterator i = MyGrid.begin();
	glColor3f(0,0,1.0);
	glBegin(GL_LINES);

	int cellcount = 0;
	while(i!=MyGrid.end()){
		cellcount++;
		Shape = (*i)->GetShape();
		LineList::iterator Si = Shape->begin();

		while(Si != Shape->end()){
			Coordinate Start = (*Si).first;
			Coordinate End = (*Si).second;
			glVertex3f(Start[0],Start[1],Start[2]);
			glVertex3f(End[0],End[1],End[2]);
			++Si;
		}
		++i;
	}
	glEnd();
}

void draw(){
	drawGrid();
	drawAABB();
}
bool initGL(){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom,1,0.1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(cam->Location[0],cam->Location[1],cam->Location[2],cam->Location[0]+(*cam->Target)[0],cam->Location[1]+(*cam->Target)[1],cam->Location[2]+(*cam->Target)[2],(*cam->Up)[0],(*cam->Up)[1],(*cam->Up)[2]);

	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	draw();
	SDL_GL_SwapBuffers();
	return true;
}

bool init(int x, int y){
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
		return false;
	}

	if ( SDL_SetVideoMode( x,y,32,SDL_OPENGL ) == NULL ){
		return false;
	}

	if ( initGL() == false ){
		return false;
	}

	SDL_WM_SetCaption( "SDL GL", NULL );
	return true; 
}

void update(){
	cam->step();
}

void render(){
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom,1,0.1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(cam->Location[0],cam->Location[1],cam->Location[2],cam->Location[0]+(*cam->Target)[0],cam->Location[1]+(*cam->Target)[1],cam->Location[2]+(*cam->Target)[2],(*cam->Up)[0],(*cam->Up)[1],(*cam->Up)[2]);


	draw();
	SDL_GL_SwapBuffers();
}
void MouseMove(int x, int y,int halfHeight, int halfWidth)
{
	int relativeX;
	int relativeY;

	if(x!=0){
    		relativeX = x - mouseX;
    		mouseX = x;
		cout<<"X"<<x<<"\n";
		cout<<"deltaX"<<relativeX<<"\n";
		cam->rotateY(relativeX/-100.0);
	}else if(y!=0){

    		relativeY = y - mouseY;
    		mouseY = y;
		cout<<"Y"<<y<<"\n";
		cout<<"deltaY"<<relativeY<<"\n";
		cam->rotateX(relativeY/100.0);
	}
		//SDL_WarpMouse(halfWidth, halfHeight);

}
int main(){
	const int screen_w = 1366;
	const int screen_h = 768;
	const int screen_bpp = 32;
	const int fps = 60;
	
	mouseX=0;
	mouseY=0;

	cam = new camera();
	MyObjects = createObjects();
	MyGrid.Add(*MyObjects);

	init(screen_w,screen_h);
	
	SDL_Surface* cobblestoneFloor;
	SDL_Render::loadimage("./6903.jpg" ,&cobblestoneFloor);	
	
	nOfColors = cobblestoneFloor->format->BytesPerPixel;
	if(nOfColors == 4){
		if(cobblestoneFloor->format->Rmask = 0x000000ff){
			texture_format = GL_RGBA;
		}else{
			texture_format = GL_BGRA;
		}
	}else if(nOfColors == 3){
		if(cobblestoneFloor->format->Rmask = 0x000000ff){
			texture_format = GL_RGB;
		}else{
			texture_format = GL_BGR;
		}
		
	}

//	glGenTextures(1,&texture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D,1);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	glTexImage2D( GL_TEXTURE_2D,0,nOfColors,cobblestoneFloor->w,cobblestoneFloor->h,0,texture_format,GL_UNSIGNED_BYTE,cobblestoneFloor->pixels);
	
	SDL_Event event;
	SDL_ShowCursor(0);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	Timer timer;
	bool quit = false;


	shared_ptr<Force> UpForce = cam->addForce(cam->Up,0);
	shared_ptr<Force> ForwardForce = cam->addForce(cam->Target,0);
	shared_ptr<Force> RightForce = cam->addForce(cam->Right,0);


	//game loop
	while(!quit){
		timer.start();

		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				quit=true;
			}else if(event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
					case SDLK_w:ForwardForce->magnitude += 0.1 ; break;
					case SDLK_s:ForwardForce->magnitude -= 0.1 ; break;
					case SDLK_d:RightForce->magnitude -= 0.1 ; break;
					case SDLK_a:RightForce->magnitude += 0.1 ; break;
					case SDLK_e:UpForce->magnitude -= 0.1 ; break;
					case SDLK_q:UpForce->magnitude +=0.1 ; break;
					case SDLK_i:cam->rotateX(5) ; break;
					case SDLK_k:cam->rotateX(-5) ; break;
					case SDLK_j:cam->rotateY(5) ; break;
					case SDLK_l:cam->rotateY(-5) ; break;
					case SDLK_z:quit=true; break;
				}
			}else if(event.type == SDL_KEYUP){
				switch(event.key.keysym.sym){
					case SDLK_w:ForwardForce->magnitude -= 0.1 ; break;
					case SDLK_s:ForwardForce->magnitude += 0.1 ; break;
					case SDLK_d:RightForce->magnitude += 0.1 ; break;
					case SDLK_a:RightForce->magnitude -= 0.1 ; break;
					case SDLK_e:UpForce->magnitude += 0.1 ; break;
					case SDLK_q:UpForce->magnitude -=0.1 ; break;
				}
			}else if(event.type == SDL_MOUSEMOTION ){
                		MouseMove(event.motion.xrel, event.motion.yrel, screen_h/2,screen_w/2);
			}


		}
		update();
		render();
		//throttle framerate
		while(timer.get_ticks()<1000/fps){}
	}

	return 0;

}
