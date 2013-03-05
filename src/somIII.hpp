#include "float.h"
#include "math.h"
#include "vectorMath.hpp"
#include <algorithm>
#include <queue>

using namespace std;

typedef vector<float> neuron;

float randomFloatInRange(float startRange, float endRange){
	float random = ((float) rand()) / (float) RAND_MAX;
	float range = endRange-startRange;
	return (random*range) + startRange;
}

class som{
	private:

		//cube dimensions
		int h;
		int w;
		int d;
		
		//for step-by-step training (allowing for rendering between)
		int train_max;
		int train_current;

		//data to train the SOM to
		vector<neuron> trainingVectors;
		queue<neuron> trainingBuffer;

		//the SOM data structure
		vector < neuron > cells;

		//training buffer lets us view a step-by-step of each training iteration
		void fillTrainingBuffer(){
			if(train_current>0){
				random_shuffle(trainingVectors.begin(),trainingVectors.end());
				for(int i = 0; i < trainingVectors.size(); i++)
					trainingBuffer.push(trainingVectors[i]);
				train_current--;
			}
		}
		
		void trainNeighbors(neuron trainingNeuron, int winner, float neigbourhoodSize, float learningCoefficient){
			//convert array index of BMU to SOM x/y/z cell coordinates
			vector<float> meshLocWinner;
			meshLocWinner.push_back((winner/(w*d))/(float)h);
			meshLocWinner.push_back( (((int)(winner/d))%w)/(float)w );
			meshLocWinner.push_back((winner%d)/(float)d);
			

			//temp variable to do the above to each SOM vector for comparison with BMU
			vector<float> meshLocCurrent;
			meshLocCurrent.push_back(0);
			meshLocCurrent.push_back(0);
			meshLocCurrent.push_back(0);
			
			float maxDistance = neigbourhoodSize;	
			for(int i = 0; i < cells.size(); i++){
				//get SOM x/y/z cell coordinates for SOM cell i
				meshLocCurrent[0] = (i/(w*d))/(float)h;
				meshLocCurrent[1] = (((int)(i/d))%w)/(float)w;
				meshLocCurrent[2] = (i%d)/(float)d;
			
				//determine if this SOM cell is in neighborhood
				float currentDistance=absDistance(meshLocCurrent,meshLocWinner);
				if(currentDistance < maxDistance){
					//if so, adjust accordingly
					float influence = ((maxDistance-currentDistance)/maxDistance)*learningCoefficient;
					cells[i] = attract(trainingNeuron,cells[i],influence);
				}
			}

		}
		
		int findBMU(neuron trainingNeuron){
			//start far away so our neural network is clearly nearer.
			float nearestDistance= FLT_MAX;
			int winningPosition;
			//cout<<"Green value:"<<trainingNeuron[1]<<"\n";
			for(int i = 0 ; i < cells.size(); i++){
				//calculate euclidian distance between training neuron and current neuron
				float currentDistance = absDistance(trainingNeuron,cells[i]);	
				if(currentDistance < nearestDistance){
					nearestDistance = currentDistance;
					winningPosition=i;
				}
			}
			return winningPosition;
		}

	public:
		//read-only access functions
		int get_h(){return h;}
		int get_w(){return w;}
		int get_d(){return d;}
		vector<float> getCell(int i){return cells[i];}

		void init(vector< neuron > t_v, int x_res, int y_res, int z_res, int tIterations){
			//save a copy of the vectors we are to train to
			trainingVectors = t_v;

			//3d placement in a 1d vector will take some array calculation
			h = y_res;
			w = x_res;
			d = z_res;

			//train_current counts down from max to zero
			train_max = train_current = tIterations;

			//3d iterative indexing
			for(int i = 0; i < h; i++){
				for(int j = 0; j < w; j++){
					for(int k = 0; k < d; k++){
						neuron myNewNeuron;
						//each som vector has the same dimensions as the training vectors
						for(int l =0; l < trainingVectors[0].size(); l++){
							myNewNeuron.push_back(randomFloatInRange(0,255)); 
						}
						cells.push_back(myNewNeuron);
					}
				}
			}

			fillTrainingBuffer();
		}

		//take a single random training vector and impress it on the SOM
		//training vectors are arranged in the queue (trainingBuffer) to ensure even distribution of training impressions
		void trainStep(){
				if(trainingBuffer.size()>0){
					vector<float> singleTrainingVector = trainingBuffer.front();
					trainingBuffer.pop();
					int BMU = findBMU(singleTrainingVector);
					trainNeighbors(singleTrainingVector, BMU, train_current/(float)train_max, train_current/(float)train_max);
				}else{
					fillTrainingBuffer();
				}
		}

		//train until the trainingBuffer is empty
		void trainPass(){
				while(trainingBuffer.size()>0){
					trainStep();
				}
				fillTrainingBuffer();
		}


		void trainAll(){
				while(train_current > 0){
					trainPass();
				}
		}
		

};

