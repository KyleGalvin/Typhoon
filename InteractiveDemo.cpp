#include <GL/glut.h>
#include "./src/hgrid.hpp"
#include "./src/collision.hpp"
#include <cstdlib>
#include <ctime>
#include <memory>
#include "SDL/SDL.h"
#include "./src/camera.hpp"

#define X_PIXELS 500
#define Y_PIXELS 500
#define Z_PIXELS 500

#define TO_RAD(x) ((x)/180.0 * 3.142592654f)

typedef Object Obj;
typedef std::shared_ptr<Obj> PtrObj;
typedef vector<PtrObj> ListPtrObj;
typedef std::shared_ptr<ListPtrObj> PtrListPtrObj;

float clocation[] = {0,0,0};
float rotation[] = {0,180,0};
float zoom = 45.0;
PtrObj FocusObj;
int lastx = 0;
int lasty = 0;

camera *cam;

PtrSetPtrObj MyObjects;
hgrid MyGrid;

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
	glColor3f(0,0.5,0);
	glBegin(GL_LINES);

	while(i !=MyObjects->end()){
		Shape=(*i)->GetShape();

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


void drawGrid(){
	typedef pair<Coordinate,Coordinate> Line;
	typedef vector<Line> LineList;
	typedef std::shared_ptr<LineList> PtrLineList;
	
	PtrLineList Shape;
	typename hgrid::const_iterator i = MyGrid.begin();
	glColor3f(0,0,0.5);
	glBegin(GL_LINES);

	int cellcount = 0;
	while(i!=MyGrid.end()){
		cellcount++;
		//cout<<"Cell: "<<cellcount<<"\n";
		Shape = (*i)->GetShape();
		LineList::iterator Si = Shape->begin();

		while(Si != Shape->end()){
			//cout<<"Lots of lines\n";
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
	//drawPoly();
	drawGrid();
	drawAABB();
}

void display(){
	SDL_Event event;
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				exit(1);
			}

			//update mouse info once we have pointer set up

		}
	//moveObjects();
	//updateCollisionGrid();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom,1,0,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cout<<cam->Target[0]<<"\n";

	gluLookAt(cam->Location[0],cam->Location[1],cam->Location[2],cam->Location[0]+cam->Target[0],cam->Location[1]+cam->Target[1],cam->Location[2]+cam->Target[2],cam->Up[0],cam->Up[1],cam->Up[2]);


	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_LIGHTING);
	draw();
	glutSwapBuffers();
}

void reshape(int x, int y){
	glViewport(0,0,x,(GLsizei)y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,0,x,y);
}

void keys(unsigned char key, int x, int y){
	if(key=='i'){
		cam->rotateX(-5);
	}if(key=='k'){
		cam->rotateX(5);
	}if(key=='l'){
		cam->rotateY(-5);
	}if(key=='j'){
		cam->rotateY(5);
	}if(key=='u'){
		cam->rotateZ(-5);
	}if(key=='o'){
		cam->rotateZ(5);
	}if(key=='w'){
		cam->translateZ(1);
	}if(key=='a'){
		cam->translateX(1);
	}if(key=='s'){
		cam->translateZ(-1);
	}if(key=='d'){
		cam->translateX(-1);
	}if(key=='q'){
		cam->translateY(-1);
	}if(key=='e'){
		cam->translateY(1);
	}if(key=='q'){
		glutLeaveGameMode();
	}

cout<<"xpos"<<cam->Location[0]<<"\n";
cout<<"ypos"<<cam->Location[1]<<"\n";
cout<<"zpos"<<cam->Location[2]<<"\n";

cout<<"xrot"<<cam->Target[0]<<"\n";
cout<<"yrot"<<cam->Target[1]<<"\n";
cout<<"zrot"<<cam->Target[2]<<"\n";

	display();
}

void mouse(int button, int state, int x, int y){
	if(button == 3){
		zoom*=0.9;
		if(zoom<10) zoom = 10;
	}else if(button == 4){
		zoom*=1.1;
		if(zoom>190) zoom = 190;
	}
	display();
}

void mousemove(int x, int y){

	/*if(x>X_PIXELS-1){
		lastx = x = 1;
	}else if(x<1){
		lastx = x = X_PIXELS-1;
	}

	if(y>Y_PIXELS-1){
		lasty = y = 1;
	}else if(y<1){
		lasty = y = Y_PIXELS-1;
	}*/
	float dx = float(x-lastx);
	float dy = float(y-lasty);
	lastx=x;
	lasty=y;
	rotation[0] += (float)dy;
	rotation[1] += (float)dx;
	//if(x != GLUT_WINDOW_WIDTH/2 && y != GLUT_WINDOW_HEIGHT/2){
	//	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	//}
	//glutSetCursor(GLUT_CURSOR_NONE);	
	display();
}

int main(int argc, char **argv){
	//MyGrid.debug = true;
	//initialize window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(X_PIXELS, Y_PIXELS); 
	//glutEnterGameMode();	
	glutCreateWindow("Space Exploder");
	glOrtho(-1,1,-1,1,-1,1);

	cam = new camera();

	//SDL_Init(SDL_INIT_EVERYTHING);
//	glutEnterGameMode();
	//define our main functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keys);
	//glutMouseFunc(mouse);
	//glutPassiveMotionFunc(mousemove);
	//initialize game elements
	MyObjects = createObjects();
	MyGrid.Add(*MyObjects);

	//run game
	glutMainLoop();
	return 0;
}
