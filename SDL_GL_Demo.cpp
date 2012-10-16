#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "./src/neuron.hpp"
#include "./src/tspFileReader.hpp"
#include "./src/objectstructs.hpp"
#include "./src/geotypes.hpp"
#include "./src/hgrid.hpp"
#include "./src/collision.hpp"
#include "./src/camera.hpp"
#include "./src/render.hpp"
#include <time.h>

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

CollisionEngine ce;

camera *cam;
PtrObj camBoundingBox;

PtrSetPtrObj MyObjects;
hgrid MyGrid;
PtrObj FocusObj;
PtrSetPtrObj SOMbricks;
vector< vector<neuron> > mySOM;

float zoom = 45.0;
int mouseX;
int mouseY;

GLuint texture;
GLint nOfColors;
GLenum texture_format;

PtrObj createCamBB(){
	PtrObj O(new AABB);
	O->Dimensions.push_back(0.1);
	O->Dimensions.push_back(0.1);
	O->Dimensions.push_back(0.1);
	O->Location.push_back(cam->Location[0]+0.05);//add half the dimension value to center box around camera
	O->Location.push_back(cam->Location[1]+0.05);
	O->Location.push_back(cam->Location[2]+0.05);
	return O;
}

PtrObj createBrick(int i,int j,float meshSize){
	PtrObj O(new AABB);
	O->Dimensions.push_back(meshSize);
	O->Dimensions.push_back(meshSize);
	O->Dimensions.push_back(meshSize);
	O->Location.push_back(i*meshSize);
	O->Location.push_back(0.1+(j*meshSize));//0.1 up pulls our SOM out of the floor
	O->Location.push_back(0);
	return O;
}

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

PtrSetPtrObj createSOMBricks(float meshSize){
	PtrSetPtrObj results(new SetPtrObj);
	int Location[3];
	int Dimensions[3];
	for(int i=0;i<20;i++){
		for(int j=0;j<20;j++){
			results->insert(createBrick(i,j,meshSize));
		}
	}
	return results;

}

PtrObj createFloor(){
	PtrObj O(new AABB);
	O->Dimensions.push_back(2);
	O->Dimensions.push_back(0.1);
	O->Dimensions.push_back(2);

	//centered at origin
	O->Location.push_back(-1);
	O->Location.push_back(0);
	O->Location.push_back(-1);
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

void drawSOM(){
	SetPtrObj::iterator i = SOMbricks->begin();
	int counter=0;
	while(i !=SOMbricks->end()){
		glColor3f(mySOM[counter/20][counter%20][0]/255.0,mySOM[counter/20][counter%20][1]/255.0,mySOM[counter/20][counter%20][2]/255.0);
		glBindTexture(GL_TEXTURE_2D,1);
		glBegin(GL_QUADS);
		(*i)->Draw3DQuads();	
		++i;
		counter++;
		glEnd();
	}
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

vector< vector<neuron> > train(vector< vector<neuron> > mySOM, vector<neuron> TrainingNeurons, int i, int iMax,bool tsp){
	if(i >0){
		for(int j =0;j < TrainingNeurons.size();j++){
			pos2d winner = findWinningNeuron(mySOM,TrainingNeurons[j]);
			
			//the travelling salesman problem uses modified SOM learning rules in order to optimize results
			if(tsp){
				mySOM = tspTrainNeigbours(mySOM, TrainingNeurons[j],winner,((float)i/(float)iMax),((float)i/(float)iMax));
			}else{
				mySOM = trainNeigbours(mySOM, TrainingNeurons[j],winner,((float)i/(float)iMax),((float)i/(float)iMax));
			}
		}
	}

	return mySOM;
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
	cout<<"drawing\n";
	drawGrid();
	cout<<"done grid\n";
	drawSOM();
	cout<<"done som\n";
	drawAABB();
	cout<<"done aabb\n";
}
bool initGL(){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom,1,0.1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam->Location[0],cam->Location[1],cam->Location[2],
		cam->Location[0]+(*cam->Target)[0],cam->Location[1]+(*cam->Target)[1],cam->Location[2]+(*cam->Target)[2],
		(*cam->Up)[0],(*cam->Up)[1],(*cam->Up)[2]);

	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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
	cout<<"updating\n";
//	MyGrid.Remove(camBoundingBox);
	cout<<"removed\n";
	cam->step();
	camBoundingBox->Location[0]=cam->Location[0];
	camBoundingBox->Location[1]=cam->Location[1];
	camBoundingBox->Location[2]=cam->Location[2];
	cout<<"changed\n";
//	MyGrid.Add(camBoundingBox);
	cout<<"replaced\n";
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
		cam->rotateY(relativeX/-100.0);
	}else if(y!=0){

    		relativeY = y - mouseY;
    		mouseY = y;
		cam->rotateX(relativeY/100.0);
	}
		//SDL_WarpMouse(halfWidth, halfHeight);

}
int main(){
	//random number generator seeded
	srand(time(NULL));

	//sdl screen info and sprite data
	SDLDrawMetaData sdl;
	SpriteData sprites;

	//setup camera
	cam = new camera();
	camBoundingBox = createCamBB();
	//move above floor!
	cam->Location[1]+=0.2;
	//camera forces we will use. Keyboard input will modify these values and move us
	shared_ptr<Force> UpForce = cam->addForce(cam->Up,0);
	shared_ptr<Force> ForwardForce = cam->addForce(cam->Target,0);
	shared_ptr<Force> RightForce = cam->addForce(cam->Right,0);

	//create 10 random objects to test hierarchal bounding tree
	MyObjects = createObjects();
	//add floor to scene
	MyObjects->insert(createFloor());
	MyGrid.Add(*MyObjects);

	//self organizing map creation
	TSPFileReader trainingVectorFactory;
	vector<neuron> TrainingNeurons;
	//we will use 3d vectors and a colour array to demo
	TrainingNeurons = trainingVectorFactory.retrieveTrainingVectors("./maps/RGB.tsp");
	//SOM will eventually be scaled between 0 and 1. here we define 20x20 grid
	float meshSize = 0.05;
	mySOM =  initializeNewSOM(meshSize,0.0,255.0);
	//SOM training counter
	int trainingIterations = 100;
	int iTrain = trainingIterations;
	SOMbricks = createSOMBricks(meshSize);
	//add our som bricks to our spatial partitioning structure (collision detection)
	MyGrid.Add(*SOMbricks);
	MyGrid.Add(camBoundingBox);

	init(sdl.screen_w,sdl.screen_h);
	//cube texture
	SDL_Surface* cobblestoneFloor;
	SDL_Render::loadimage("./6903.jpg" ,&cobblestoneFloor);	
	
	//transfer SDL surface into openGL texture
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
	//openGL texturing and depth map configuration
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
	

	//sdl cursor initialization
	mouseX=0;
	mouseY=0;
	SDL_Event event;
	SDL_ShowCursor(0);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	bool quit = false;



	//game loop
	while(!quit){

		//keyboard/mouse handler
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
                		MouseMove(event.motion.xrel, event.motion.yrel, sdl.screen_h/2,sdl.screen_w/2);
			}


		}
		update();
		if(iTrain>0){
			iTrain--;
			mySOM = train(mySOM,TrainingNeurons,iTrain,trainingIterations,false);
		}
		render();
	}

	return 0;

}
