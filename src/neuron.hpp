#include <vector>
#include <fstream>
#include <string>
#include "float.h"
#include "math.h"
#include "vectorMath.hpp"
#include <iostream>
using namespace std;

typedef vector<float> neuron;
typedef vector< vector<neuron> > som; //2d neighborhood

struct pos2d{
	int x;
	int y;
};

vector< neuron > createHierarchalMeshData(float x, float y){
	vector<neuron> results;
			neuron NewTrainingData;
	for(int i=0; i<x;i++){
		for(int j=0; j<y; j++){
			NewTrainingData.push_back((1/x)*(float)i);
			NewTrainingData.push_back((1/y)*(float)j);
			results.push_back(NewTrainingData);
			NewTrainingData.clear();
		}
	}

	//add clustered data
	for(int i=0; i<x;i++){
		for(int j=0; j<y; j++){
			NewTrainingData.push_back(0.75+(1/x)*(float)i/x);
			NewTrainingData.push_back(0.75+(1/y)*(float)j/y);
			results.push_back(NewTrainingData);
			NewTrainingData.clear();
		}
	}

	return results;
}

vector< neuron > createMeshData(float x, float y){
	vector<neuron> results;
			neuron NewTrainingData;
	for(int i=0; i<x;i++){
		for(int j=0; j<y; j++){
			NewTrainingData.push_back((1/x)*(float)i);
			NewTrainingData.push_back((1/y)*(float)j);
			results.push_back(NewTrainingData);
			NewTrainingData.clear();
		}
	}

	return results;
}

pos2d findWinningNeuron(som mySOM, neuron trainingNeuron){
	//start far away so our neural network is clearly nearer.
	float nearestDistance= FLT_MAX;
	neuron* result;
	pos2d winningPosition;
	for(int i = 0 ; i < mySOM.size(); i++){
		for(int j = 0; j < mySOM[i].size(); j++){
			//calculate euclidian distance between training neuron and current neuron
			float currentDistance = absDistance(trainingNeuron,mySOM[i][j]);	
			if(currentDistance < nearestDistance){
				nearestDistance = currentDistance;
				winningPosition.x=i;
				winningPosition.y=j;
			}
		}
	}
	return winningPosition;
}

som tspTrainNeigbours(som mySOM, neuron trainingNeuron, pos2d winner, float neigbourhoodSize, float learningCoefficient){
	vector<float> meshLocWinner;
	meshLocWinner.push_back((float)winner.x/(float)mySOM.size());
	meshLocWinner.push_back((float)winner.y/(float)mySOM[0].size());

	//placeholders will be overwritten, but we determine 2d here
	vector<float> meshLocCurrent;
	meshLocCurrent.push_back(0);
	meshLocCurrent.push_back(0);
	vector<float> centerScreen;
	centerScreen.push_back(0.5);
	centerScreen.push_back(0.5);
	//we want the neighborhood to start over the radius = 0.5 from winner and decrease
	float maxDistance = neigbourhoodSize;	
	//iterate over all neurons
	for(int i = 0; i < mySOM.size(); i++){
		for(int j = 0 ; j < mySOM[0].size(); j++){
			meshLocCurrent[0] = (float)i/(float)mySOM.size();
			meshLocCurrent[1] = (float)j/(float)mySOM[0].size();
			float currentDistance=absDistance(meshLocCurrent,meshLocWinner);
			if(currentDistance < maxDistance){
				float influence = ((maxDistance-currentDistance)/maxDistance)*learningCoefficient;
				mySOM[i][j] = attract(trainingNeuron,mySOM[i][j],influence);
			
				mySOM[i][j] = repel(centerScreen,mySOM[i][j],influence);
				
				mySOM[i][j] = attract(midpoint(mySOM[i][(j+1)%mySOM[0].size()],
					mySOM[0][((j-1)%mySOM[0].size())]),mySOM[i][j],influence);
			}
		}
	}

	mySOM[winner.x][winner.y] = attract(trainingNeuron,mySOM[winner.x][winner.y],1.0);

	return mySOM;	
}

som trainNeigbours(som mySOM, neuron trainingNeuron, pos2d winner, float neigbourhoodSize, float learningCoefficient){
	vector<float> meshLocWinner;
	meshLocWinner.push_back((float)winner.x/(float)mySOM.size());
	meshLocWinner.push_back((float)winner.y/(float)mySOM[0].size());

	//placeholders will be overwritten, but we determine 2d here
	vector<float> meshLocCurrent;
	meshLocCurrent.push_back(0);
	meshLocCurrent.push_back(0);

	//we want the neighborhood to start over the radius = 0.5 from winner and decrease
	float maxDistance = neigbourhoodSize;	
	//iterate over all neurons
	for(int i = 0; i < mySOM.size(); i++){
		for(int j = 0 ; j < mySOM[0].size(); j++){
			meshLocCurrent[0] = (float)i/(float)mySOM.size();
			meshLocCurrent[1] = (float)j/(float)mySOM[0].size();
			float currentDistance=absDistance(meshLocCurrent,meshLocWinner);
			if(currentDistance < maxDistance){
				float influence = ((maxDistance-currentDistance)/maxDistance)*learningCoefficient;
				mySOM[i][j] = attract(trainingNeuron,mySOM[i][j],influence);
			}
		}
	}

	return mySOM;	
}

vector<neuron> scaleNeuronsToRange(vector<neuron> Neurons, float min, float max){
	//start values at extremes so our data will definitely take the placeholders
	float currentMinX=FLT_MAX;
	float currentMaxX= 0;
	float currentMinY=FLT_MAX;
	float currentMaxY= 0;

	//find most extreme values
	for(int i = 0; i<Neurons.size(); i++){
			if(Neurons[i][0] > currentMaxX)
				currentMaxX = Neurons[i][0];
			if(Neurons[i][0] < currentMinX)
				currentMinX = Neurons[i][0];
			if(Neurons[i][1] > currentMaxY)
				currentMaxY = Neurons[i][1];
			if(Neurons[i][1] < currentMinY)
				currentMinY = Neurons[i][1];
	}
	//translate values so the minimum X and Y values are at 0
	currentMaxX -= currentMinX;
	currentMaxY -= currentMinY;
	for(int i = 0; i<Neurons.size(); i++){
		Neurons[i][0] -= currentMinX;
		Neurons[i][1] -= currentMinY;
	}

	//scale all values to fit extremes within range [min,max]
	float absMax;
	absMax = ((currentMaxX > currentMaxY) ? currentMaxX : currentMaxY);
	for(int i = 0; i<Neurons.size(); i++){
		Neurons[i][0] /= absMax;
		Neurons[i][1] /= absMax;
	}

	return Neurons;

}

float RandomFloatInRange(float startRange, float endRange){
	float random = ((float) rand()) / (float) RAND_MAX;
	float range = endRange-startRange;
	return (random*range) + startRange;
}

vector< vector<neuron> > initializeNewSOM(float meshSize,float min, float max){

	vector< vector<neuron> > results;

	for(float i = 0.0; i < 1.0; i+=meshSize){
		//new row
		vector<neuron> row;
		for(float j = 0.0; j < 1.0; j+=meshSize){
			//new column
			neuron NewNeuron;
			NewNeuron.push_back(RandomFloatInRange(min,max));
			NewNeuron.push_back(RandomFloatInRange(min,max));
			row.push_back(NewNeuron);
		}
		results.push_back(row);
	}
	return results;
}

//beginning with random data isn't optimal for approximating TSP solution
//circular starting data for the TSP problem. circle center = (0.5,0.5), radius = 0.5 to fill map
vector< vector<neuron> > initializeCircularSOM(int neuronCount){
	vector<neuron> results;
	
	float centerX = 0.5;
	float centerY = 0.5;
	float neuronX = 0.9;
	float neuronY = 0.5;
	float pi = 3.14159265;

	//translate center of neuron radius to origin
	float tempNeuronX = neuronX-centerX;
	float tempNeuronY = neuronY-centerY;

	float degreeInc = 360.0 / (neuronCount);

	for(int i = 0; i <neuronCount; i++){
		neuron newNeuron;
		//x val. Before saving it, we translate back from origin to proper center
		newNeuron.push_back((tempNeuronX * cos(degreeInc*i*pi/180) + tempNeuronY * sin(degreeInc*i*pi/180))+centerX);
		//y val. same as above
		newNeuron.push_back((-1*tempNeuronX * sin(degreeInc*i*pi/180) + tempNeuronY * cos(degreeInc*i*pi/180))+centerY);
		results.push_back(newNeuron);
	}

	//TSP is 1d problem, SOM data structure is 2d. 
	//here we wrap a 1d problem into 2d space to keep data structure consistent with library
	vector< vector< neuron> > wrapper;
	wrapper.push_back(results);
	return wrapper;
}
