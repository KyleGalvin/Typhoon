
//OpenGL
#include <GL/glut.h>
#include "SDL/SDL_opengl.h"

//Collision Detection
#include "./src/objectstructs.hpp"
#include "./src/geotypes.hpp"
#include "./src/hgrid.hpp"
//#include "./src/collision.hpp"

//Dynamic Camera Class
#include "./src/camera.hpp"

//Self-Organizing Map
//#include "./src/neuron.hpp"
//#include "./src/tspFileReader.hpp"

//SDL Windowing Tools
#include "SDL/SDL.h"
#include "./src/render.hpp"

//OpenCV
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

//OpenNI
#include <ni/XnCppWrapper.h>

//System clock ticker
#include <time.h>

using namespace cv;
using namespace xn;
using namespace std;

#define X_PIXELS 500
#define Y_PIXELS 500
#define Z_PIXELS 500
#define SAMPLE_XML_PATH "./config/SampleKinectConfig.xml"

struct SDLDrawMetaData{
	static const int screen_w=500;
	static const int screen_h=500;
	static const int screen_bpp=32;
	static const int fps=60;
	SDL_Surface *screen;
};

Context g_context;
ScriptNode g_scriptNode;
DepthGenerator g_depth;
ImageGenerator g_image;
DepthMetaData g_depthMD;
ImageMetaData g_imageMD;

CollisionEngine ce;

camera *cam;
PtrObj camBoundingBox;

VideoCapture capture(CV_CAP_OPENNI); // or CV_CAP_OPENNI
PtrSetPtrObj MyObjects;
PtrSetPtrObj BrickArray;
hgrid MyGrid;
PtrObj FocusObj;

bool draw_AABB;
bool draw_HGrid;

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

PtrSetPtrObj createBrickArray(int x, int y){
	PtrSetPtrObj results(new SetPtrObj);
	int Location[3];
	int Dimensions[3];
	for(int i=0;i<x;i++){
		for(int j=0;j<y;j++){
			results->insert(createBrick(i,j,0.005));
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

void drawBrickGrid(Mat bgrImage, Mat depthMap){

	typedef pair<Coordinate,Coordinate> Line;
	typedef vector<Line> LineList;
	typedef std::shared_ptr<LineList> PtrLineList;
	
	SetPtrObj::iterator i = BrickArray->begin();
	PtrLineList Shape;

	int rows=bgrImage.rows;
	int columns=bgrImage.cols;
	int ri=0;
	int ci=0;
	cout<<"Drawing Grid!\n";
	while(i !=BrickArray->end()){
		glPushMatrix();
		glTranslatef((*i)->Location[0],(*i)->Location[1],(*i)->Location[2]);
		//start drawin lines!
		cout<<"Drawing Column\n";
		glColor3f(bgrImage.at<Vec3b>(ri,ci)[2],bgrImage.at<Vec3b>(ri,ci)[1],bgrImage.at<Vec3b>(ri,ci)[0]);
		cout<<"done finding colour\n";
		ci++;
		if(ci==columns){ci==0;ri++;}
		
		//glBindTexture(GL_TEXTURE_2D,1);
		glBegin(GL_QUADS);
		glutSolidCube((*i)->Dimensions[0]);
		++i;
		glEnd();
		glPopMatrix();
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

void drawGrid(){

	typedef pair<Coordinate,Coordinate> Line;
	typedef vector<Line> LineList;
	typedef std::shared_ptr<LineList> PtrLineList;
	
	PtrLineList Shape;
	typename hgrid::const_iterator i = MyGrid.begin();
	glBegin(GL_LINES);
	glColor3f(0.0,1.0,0.0);

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
	Mat depthMap;
	Mat bgrImage;

	if(!capture.grab())
	{
		cout<<"\n Not able to grab images \n";
		exit(1);
	}
 
	capture.retrieve( depthMap, CV_CAP_OPENNI_DEPTH_MAP );
	capture.retrieve( bgrImage, CV_CAP_OPENNI_BGR_IMAGE );
	cout<<bgrImage.at<Vec3b>(0,0)[0]<<"<==color\n";
	drawBrickGrid(bgrImage,depthMap);
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
//	MyGrid.Remove(camBoundingBox);
	cam->step();
	camBoundingBox->Location[0]=cam->Location[0];
	camBoundingBox->Location[1]=cam->Location[1];
	camBoundingBox->Location[2]=cam->Location[2];
//	MyGrid.Add(camBoundingBox);
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
int main(int argc, char** argv){
	printf("Program Init...");
	XnStatus rc;

	EnumerationErrors errors;
	rc = g_context.InitFromXmlFile(SAMPLE_XML_PATH, g_scriptNode, &errors);

	std::cout<<"Kinect Init...";
	g_depth.GetMetaData(g_depthMD);
	g_image.GetMetaData(g_imageMD);

	std::cout<<"Kinect Data...";
Context context;
context.Init();
assert(capture.isOpened()); 

	//setup camera
	cam = new camera();
	cam->Location[0]=1.5;
	cam->Location[1]=1;
	cam->Location[2]=-4;
	camBoundingBox = createCamBB();
	shared_ptr<Force> UpForce = cam->addForce(cam->Up,0);
	shared_ptr<Force> ForwardForce = cam->addForce(cam->Target,0);
	shared_ptr<Force> RightForce = cam->addForce(cam->Right,0);

	std::cout<<"Camera Init...";
	BrickArray = createBrickArray(640,480);
	if(capture.get( CV_CAP_PROP_OPENNI_REGISTRATION ) == 0) capture.set(CV_CAP_PROP_OPENNI_REGISTRATION,1);
	cout<<"past init\n";
	SDLDrawMetaData sdl;	
	std::cout<<"SDL Init 1/2...";
 	glutInit(&argc, argv);
	std::cout<<"OpenGL Init...";
	init(sdl.screen_w,sdl.screen_h);
	
	std::cout<<"Voxel Grid Init...";
	SDL_Event event;
	SDL_ShowCursor(0);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	bool quit = false;

	std::cout<<"SDL Init 2/2...";


	//game loop
	while(!quit){

		//keyboard/mouse handler
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				quit=true;
			}else if(event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
					case SDLK_w:ForwardForce->magnitude += 0.01 ; break;
					case SDLK_s:ForwardForce->magnitude -= 0.01 ; break;
					case SDLK_d:RightForce->magnitude -= 0.01 ; break;
					case SDLK_a:RightForce->magnitude += 0.01 ; break;
					case SDLK_e:UpForce->magnitude -= 0.01 ; break;
					case SDLK_q:UpForce->magnitude +=0.01 ; break;
					case SDLK_i:cam->rotateX(5) ; break;
					case SDLK_k:cam->rotateX(-5) ; break;
					case SDLK_j:cam->rotateY(5) ; break;
					case SDLK_l:cam->rotateY(-5) ; break;
					case SDLK_o:cam->rotateZ(5) ; break;
					case SDLK_u:cam->rotateZ(-5) ; break;
					case SDLK_z:quit=true; break;
				}
			}else if(event.type == SDL_KEYUP){
				switch(event.key.keysym.sym){
					case SDLK_w:ForwardForce->magnitude -= 0.01 ; break;
					case SDLK_s:ForwardForce->magnitude += 0.01 ; break;
					case SDLK_d:RightForce->magnitude += 0.01 ; break;
					case SDLK_a:RightForce->magnitude -= 0.01 ; break;
					case SDLK_e:UpForce->magnitude += 0.01 ; break;
					case SDLK_q:UpForce->magnitude -=0.01 ; break;
				}
			}else if(event.type == SDL_MOUSEMOTION ){
                //		MouseMove(event.motion.xrel, event.motion.yrel, sdl.screen_h/2,sdl.screen_w/2);
			}


		}
		update();
		render();
	}

	return 0;

}

