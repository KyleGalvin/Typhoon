//3d SOM model
#include "somIII.hpp"

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

		//sdl event pipeline handled by controller
		bool terminateProgram;	
		SDL_Event event;

		void init(){
	
			//use time to get more random initial SOM arrangement
			srand(time(NULL));

			TSPFileReader trainingVectorFactory;
			vector<neuron> TrainingNeurons;

			TrainingNeurons = trainingVectorFactory.retrieveTrainingVectors("./maps/RGB.tsp");

			cam = new camera();
			cam->translateRight(0.75);
			cam->translateUp(0.75);
			cam->translateForward(-1.75);

			//colourcube: 3d data, 3d mesh(10x10x10), 1000 training iterations
			mySOM.init(TrainingNeurons,17,11,13,100);

			terminateProgram = false;
		}
};
