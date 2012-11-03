#include "SDL/SDL.h"
#include "./src/render.hpp"
#include <iostream>
#include <vector>
#include "./src/ghsom.hpp"
#include "./src/tspFileReader.hpp"
#include <SDL/SDL_gfxPrimitives.h>
#include <algorithm>
#include <time.h>
using namespace std;

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

void drawGridArrangement(float meshSize, SDLDrawMetaData sdl, map mySOM, 
SpriteData sprites, vector<neuron> TrainingNeurons){

	SDL_Render::mergesurface(0,0,&sprites.background,&sdl.screen);
	for(int i=0;i<mySOM.size();i++){
		for(int j=0;j<mySOM[0].size();j++){
			//draw network neighbourhood relations
			//circular array (last element +1 is first element)
			if(i==mySOM.size()-1){//wrap columns into taurus
			}else{
				lineRGBA(sdl.screen,mySOM[i][j].SOMDataVector[0]*sdl.screen_w,mySOM[i][j].SOMDataVector[1]*sdl.screen_h,mySOM[i+1][j].SOMDataVector[0]*sdl.screen_w,
				mySOM[i+1][j].SOMDataVector[1]*sdl.screen_h,255,255,255,255);
			}
			if(j==mySOM[0].size()-1){//wrap rows into taurus
			}else{
				lineRGBA(sdl.screen,mySOM[i][j].SOMDataVector[0]*sdl.screen_w,mySOM[i][j].SOMDataVector[1]*sdl.screen_h,mySOM[i][j+1].SOMDataVector[0]*sdl.screen_w,
				mySOM[i][j+1].SOMDataVector[1]*sdl.screen_h,255,255,255,255);
			}
			//draw neurons in red, training vectors in green(adjust visuals for neuron sprite height/width of 10... subtract 5)
			SDL_Render::mergesurface((int)(mySOM[i][j].SOMDataVector[0]*sdl.screen_w)-5,(int)(mySOM[i][j].SOMDataVector[1]*sdl.screen_h)-5,
			&sprites.somMarkerStamp,&sdl.screen);
		}
	}
	for(int i = 0; i < TrainingNeurons.size();i++){
		SDL_Render::mergesurface((int)(TrainingNeurons[i][0]*sdl.screen_w)-5,(int)(TrainingNeurons[i][1]*sdl.screen_h)-5,
		&sprites.trainMarkerStamp,&sdl.screen);
	}
}

void drawColorCubes(float meshSize, SDLDrawMetaData sdl, vector< vector <region> > mySOM, SpriteData sprites){
	SDL_Render::mergesurface(0,0,&sprites.background,&sdl.screen);
	for(int i=0;i<mySOM.size();i++){
		for(int j=0;j<mySOM[0].size();j++){
			boxRGBA(sdl.screen, meshSize*sdl.screen_w*i, meshSize*sdl.screen_h*j,meshSize*sdl.screen_w*(i+1), 
			meshSize*sdl.screen_h*(j+1), mySOM[i][j].SOMDataVector[0], mySOM[i][j].SOMDataVector[1], mySOM[i][j].SOMDataVector[2], 255 );
		}
	}
}

int main(){
	//use time to get more random initial SOM arrangement
	srand(time(NULL));
	SDLDrawMetaData sdl;

	SDL_Event event;

	SpriteData sprites;
	
	bool quit = false;

	SDL_Render::initscreen(sdl.screen_w,sdl.screen_h,sdl.screen_bpp,&sdl.screen);
	SDL_Render::loadimage("./sprites/testbackimage.png",&sprites.background);
	SDL_Render::loadimage("./sprites/marker.png",&sprites.somMarkerStamp);
	SDL_Render::loadimage("./sprites/trainmarker.png",&sprites.trainMarkerStamp);
	SDL_Render::loadimage("./sprites/nearestmarker.png",&sprites.nearestMarkerStamp);

	string map;
	std::cout<<"MAPS:\n";
	std::cout<<"[1] RGB (3D)\n";
	std::cout<<"[2] clustered uniform mesh (2D HSOM - incomplete)\n";
	std::cout<<"Please enter map ID:\n";
	getline(cin,map);

	TSPFileReader trainingVectorFactory;
	vector<neuron> TrainingNeurons;

	float meshSize = 0.05;
	float tau1 = 0.1;//breadth of each som - expanding map
	float tau2 = 0.1;//depth of som hierarchy - recursive map

	if(map.find('1')!=string::npos){
		TrainingNeurons = trainingVectorFactory.retrieveTrainingVectors("./maps/RGB.tsp");
	}else if(map.find('2')!=string::npos){
		TrainingNeurons = createHierarchalMeshData(1/meshSize,1/meshSize);
	}

	vector< vector<region> > mySOM;

	if(map.find('2')!=string::npos){
		//mesh: 2d data, 2d mesh
		//level1 ghsom starts with 4 nodes, hence 0.5, as 1/(0.5) = 2 nodes for each x and y data dimensions
		mySOM =  initializeNewSOM(0.5,0.0,1.0);
	}else{
		//coloursquare: 3d data, 2d mesh
		mySOM =  initializeNewSOM(meshSize,0.0,255.0);
	}
	int trainingIterations = 100;

	int trainingVectorLength = TrainingNeurons[0].size();

	mySOM = train(mySOM,trainingIterations,TrainingNeurons);
	
	//game loop
	while(!quit){

		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				quit=true;
			}
			//update mouse info once we have pointer set up

		}
		if(trainingVectorLength == 3){
			drawColorCubes(meshSize,sdl,mySOM,sprites);
		}else if(trainingVectorLength == 2 && map.find('2')!=string::npos){
			drawGridArrangement(meshSize, sdl, mySOM, sprites,TrainingNeurons);
		}else{
			cout<<"bogus training data!\n";
		}

		if(SDL_Flip(sdl.screen)==-1){
			cout<<"Screen failed to flip\n";
		}

	}
	return 0;
}

