//3d SOM model
#include "somIII.hpp"

//for hierarchal som navigation
#include "somStack.hpp"

//simplified adapter/wrapper for SDL library window creation & 2d drawing functions
//SDL.h and SDL_image.h included internally, which includes SDL_Event for our controller routines
#include "render.hpp"

//library for rendering an openGL context to an SDL window
#include "SDL/SDL_opengl.h"

//3d viewing model
#include "camera.hpp"

//import SOM training vectors during initialization
#include "tspFileReader.hpp"

class model{
	public:
		//self-organizing map model is our focus
		som mySOM;

		//camera class for viewing som model
		camera* cam;

		somStack mySomStack;

		//sdl event pipeline handled by controller
		bool terminateProgram;	
		SDL_Event event;

		//clicking in the window sets our mouse x/y offset here, and triggers the view to calculate
		//any cell collisions with the mouse vector. If collisions found, the nearest is placed here
		//Otherwise, selectedIndex < 0 (=-1).
		//mouseX and mouseY are written to by the SDL Controller only
		//selectedIndex is written to by the viewer only. This is done after the view reads mouse x/y
		//once selectedIndex is set to a real value (>0), who acts on it? how/when?
		int selectedIndex;
		int mouseX;
		int mouseY;

		void init(){
			mouseX = 0;
			mouseY = 0;
			selectedIndex = -1;	
			//use time to get more random initial SOM arrangement
			srand(time(NULL));

			TSPFileReader trainingVectorFactory;
			vector<neuron> TrainingNeurons;

			//TrainingNeurons = trainingVectorFactory.retrieveTrainingVectors("./maps/RGB.tsp");
	
			neuron myNewNeuron;
			//data randomly generated
			for(int i=0; i<20; i++){
				for (int j=0;j<3;j++){
					myNewNeuron.push_back(randomFloatInRange(0,255)); 
				}
				TrainingNeurons.push_back(myNewNeuron);
				myNewNeuron.clear();
			}

			//clustered data added
			for(int i=0; i<100; i++){
				for(int j=0;j<3;j++){
					//add only high-blue values
					if(j==0){
						myNewNeuron.push_back(randomFloatInRange(0,255)); 
					}else{
						myNewNeuron.push_back(0); 
					}
				}
				TrainingNeurons.push_back(myNewNeuron);
				myNewNeuron.clear();
			}
			//clustered data added
			for(int i=0; i<100; i++){
				for(int j=0;j<3;j++){
					//add only high-blue values
					if(j==1){
						myNewNeuron.push_back(randomFloatInRange(0,255)); 
					}else{
						myNewNeuron.push_back(0); 
					}
				}
				TrainingNeurons.push_back(myNewNeuron);
				myNewNeuron.clear();
			}
			//clustered data added
			for(int i=0; i<100; i++){
				for(int j=0;j<3;j++){
					//add only high-blue values
					if(j==2){
						myNewNeuron.push_back(randomFloatInRange(0,255)); 
					}else{
						myNewNeuron.push_back(0); 
					}
				}
				TrainingNeurons.push_back(myNewNeuron);
				myNewNeuron.clear();
			}

			cam = new camera();
			cam->translateRight(0.75);
			cam->translateUp(0.75);
			cam->translateForward(-1.75);


			//colourcube: 3d data, 3d mesh(10x10x10), 1000 training iterations
			mySOM.init(TrainingNeurons,7,7,7,50);

			terminateProgram = false;
		}
};
