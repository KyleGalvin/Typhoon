#include "SDL/SDL.h"
#include "./src/render.hpp"
#include <iostream>
#include <vector>
#include "./src/neuron.hpp"
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

void drawGridArrangement(float meshSize, SDLDrawMetaData sdl, vector< vector <neuron> > mySOM, 
SpriteData sprites, vector<neuron> TrainingNeurons){

	SDL_Render::mergesurface(0,0,&sprites.background,&sdl.screen);
	for(int i=0;i<mySOM.size();i++){
		for(int j=0;j<mySOM[0].size();j++){
			//draw network neighbourhood relations
			//circular array (last element +1 is first element)
			if(i==mySOM.size()-1){//wrap columns into taurus
			}else{
				lineRGBA(sdl.screen,mySOM[i][j][0]*sdl.screen_w,mySOM[i][j][1]*sdl.screen_h,mySOM[i+1][j][0]*sdl.screen_w,
				mySOM[i+1][j][1]*sdl.screen_h,255,255,255,255);
			}
			if(j==mySOM[0].size()-1){//wrap rows into taurus
			}else{
				lineRGBA(sdl.screen,mySOM[i][j][0]*sdl.screen_w,mySOM[i][j][1]*sdl.screen_h,mySOM[i][j+1][0]*sdl.screen_w,
				mySOM[i][j+1][1]*sdl.screen_h,255,255,255,255);
			}
			//draw neurons in red, training vectors in green(adjust visuals for neuron sprite height/width of 10... subtract 5)
			SDL_Render::mergesurface((int)(mySOM[i][j][0]*sdl.screen_w)-5,(int)(mySOM[i][j][1]*sdl.screen_h)-5,
			&sprites.somMarkerStamp,&sdl.screen);
		}
	}
	for(int i = 0; i < TrainingNeurons.size();i++){
		SDL_Render::mergesurface((int)(TrainingNeurons[i][0]*sdl.screen_w)-5,(int)(TrainingNeurons[i][1]*sdl.screen_h)-5,
		&sprites.trainMarkerStamp,&sdl.screen);
	}
}

//draw 2d SOM vectors and 2d training vectors as sprites on our screens x/y plain. Draw neighbourhood relation of SOM vectors as lineRGBA's
//the first SOM node is also circularly linked to the last som node (hence the name taurus)
void drawTaurusArrangement(float meshSize, SDLDrawMetaData sdl, vector< vector <neuron> > mySOM, 
SpriteData sprites, vector<neuron> TrainingNeurons){

	SDL_Render::mergesurface(0,0,&sprites.background,&sdl.screen);
	for(int i=0;i<mySOM.size();i++){
		for(int j=0;j<mySOM[0].size();j++){
			//draw network neighbourhood relations
			//circular array (last element +1 is first element)
			if(i==mySOM.size()-1){//wrap columns into taurus
				lineRGBA(sdl.screen,mySOM[i][j][0]*sdl.screen_w,mySOM[i][j][1]*sdl.screen_h,mySOM[0][j][0]*sdl.screen_w,
				mySOM[0][j][1]*sdl.screen_h,255,255,255,255);
			}else{
				lineRGBA(sdl.screen,mySOM[i][j][0]*sdl.screen_w,mySOM[i][j][1]*sdl.screen_h,mySOM[i+1][j][0]*sdl.screen_w,
				mySOM[i+1][j][1]*sdl.screen_h,255,255,255,255);
			}
			if(j==mySOM[0].size()-1){//wrap rows into taurus
				lineRGBA(sdl.screen,mySOM[i][j][0]*sdl.screen_w,mySOM[i][j][1]*sdl.screen_h,mySOM[i][0][0]*sdl.screen_w,
				mySOM[i][0][1]*sdl.screen_h,255,255,255,255);
			}else{
				lineRGBA(sdl.screen,mySOM[i][j][0]*sdl.screen_w,mySOM[i][j][1]*sdl.screen_h,mySOM[i][j+1][0]*sdl.screen_w,
				mySOM[i][j+1][1]*sdl.screen_h,255,255,255,255);
			}
			//draw neurons in red, training vectors in green(adjust visuals for neuron sprite height/width of 10... subtract 5)
			SDL_Render::mergesurface((int)(mySOM[i][j][0]*sdl.screen_w)-5,(int)(mySOM[i][j][1]*sdl.screen_h)-5,
			&sprites.somMarkerStamp,&sdl.screen);
		}
	}

	for(int i = 0; i < TrainingNeurons.size();i++){
		SDL_Render::mergesurface((int)(TrainingNeurons[i][0]*sdl.screen_w)-5,(int)(TrainingNeurons[i][1]*sdl.screen_h)-5,
		&sprites.trainMarkerStamp,&sdl.screen);
	}
}

void drawColorCubes(float meshSize, SDLDrawMetaData sdl, vector< vector <neuron> > mySOM, SpriteData sprites){
	SDL_Render::mergesurface(0,0,&sprites.background,&sdl.screen);
	for(int i=0;i<mySOM.size();i++){
		for(int j=0;j<mySOM[0].size();j++){
			boxRGBA(sdl.screen, meshSize*sdl.screen_w*i, meshSize*sdl.screen_h*j,meshSize*sdl.screen_w*(i+1), 
			meshSize*sdl.screen_h*(j+1), mySOM[i][j][0], mySOM[i][j][1], mySOM[i][j][2], 255 );
		}
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
	std::cout<<"[0] RGB (3D)\n";
	std::cout<<"[1] burma14 (TSP)\n";
	std::cout<<"[2] eil51 (TSP)\n";
	std::cout<<"[3] d2103 (TSP)\n";
	std::cout<<"[4] uniform mesh (2D)\n";
	std::cout<<"[5] clustered uniform mesh (2D HSOM - incomplete)\n";
	std::cout<<"Please enter map ID:\n";
	getline(cin,map);

	TSPFileReader trainingVectorFactory;
	vector<neuron> TrainingNeurons;

	float meshSize = 0.05;

	if(map.find('0')!=string::npos){
		TrainingNeurons = trainingVectorFactory.retrieveTrainingVectors("./maps/RGB.tsp");
	}else if(map.find('1')!=string::npos){
		TrainingNeurons = trainingVectorFactory.retrieveTrainingVectors("./maps/burma14.tsp");
	}else if(map.find('2')!=string::npos){
		TrainingNeurons = trainingVectorFactory.retrieveTrainingVectors("./maps/eil51.tsp");
	}else if(map.find('3')!=string::npos){
		TrainingNeurons = trainingVectorFactory.retrieveTrainingVectors("./maps/d2103.tsp");
	}else if(map.find('4')!=string::npos){
		TrainingNeurons = createMeshData(1/meshSize,1/meshSize);
	}else if(map.find('5')!=string::npos){
		TrainingNeurons = createHierarchalMeshData(1/meshSize,1/meshSize);
	}

	vector< vector<neuron> > mySOM;

	if(map.find('4')!=string::npos || map.find('5')!=string::npos){
		//mesh: 2d data, 2d mesh
//		TrainingNeurons = scaleNeuronsToRange(TrainingNeurons,0.0,1.0);
		mySOM =  initializeNewSOM(meshSize,0.0,1.0);
	}else if(TrainingNeurons[0].size()==2){
		//TSP: 2d data, 1d mesh
		TrainingNeurons = scaleNeuronsToRange(TrainingNeurons,0.0,1.0);
		mySOM =  initializeCircularSOM(TrainingNeurons.size()*2);
	}else{
		//coloursquare: 3d data, 2d mesh
		mySOM =  initializeNewSOM(meshSize,0.0,255.0);
	}
	int trainingIterations = 100;
	int iTrain = trainingIterations;

	int trainingVectorLength = TrainingNeurons[0].size();
	//game loop
	while(!quit){

		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				quit=true;
			}
			//update mouse info once we have pointer set up

		}
		random_shuffle(TrainingNeurons.begin(),TrainingNeurons.end());
		//we visualize 3d data with colored boxes
		if(trainingVectorLength == 3){
			drawColorCubes(meshSize,sdl,mySOM,sprites);
		}else if(trainingVectorLength == 2 && map.find('4')!=string::npos){
			drawGridArrangement(meshSize, sdl, mySOM, sprites,TrainingNeurons);
		}else if(trainingVectorLength == 2 && map.find('5')!=string::npos){
			drawGridArrangement(meshSize, sdl, mySOM, sprites,TrainingNeurons);
		}else if(trainingVectorLength==2){
			drawTaurusArrangement(meshSize, sdl, mySOM, sprites,TrainingNeurons);
		}else{
			cout<<"bogus training data!\n";
		}


		if(map.find('4')==string::npos && map.find('5')==string::npos  && TrainingNeurons[0].size()==2){
			//TSP. winner takes all (neighbors take some)
			mySOM = train(mySOM,TrainingNeurons,iTrain,trainingIterations,true);
		}else{
			//2D mesh winner takes most (neighbors take some)
			mySOM = train(mySOM,TrainingNeurons,iTrain,trainingIterations,false);
		}

		if(SDL_Flip(sdl.screen)==-1){
			cout<<"Screen failed to flip\n";
		}

		iTrain--;
	}
	return 0;
}

