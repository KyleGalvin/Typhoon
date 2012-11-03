#include <vector>
#include <fstream>
#include <string>
#include "float.h"
#include "math.h"
#include "vectorMath.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

typedef vector<float> neuron;

//forward-declared to fix cyclical dependencies in union region
struct som;
struct region;

typedef vector< vector<region> > map; //2d neighborhood

struct som{
	map dataSet;//map to be trained
	vector<neuron> inputVectors;//input vectors to train with
	neuron mqe;//mean quantization error
	neuron center;//focus of the map (average of input vectors)
};

struct region{
	neuron SOMDataVector;
	som myNestedSOM;
	bool leaf;//leaf node will have data vector, otherwise mySOM (like a union, only one defined)
};

struct pos2d{
	int x;
	int y;
};

map trainNeigbours(map mySOM, neuron trainingNeuron, pos2d winner, float neigbourhoodSize, float learningCoefficient){
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
				mySOM[i][j].SOMDataVector = attract(trainingNeuron,mySOM[i][j].SOMDataVector,influence);
			}
		}
	}

	return mySOM;	
}

pos2d findWinningNeuron(map mySOM, neuron trainingNeuron){
	//start far away so our neural network is clearly nearer.
	float nearestDistance= FLT_MAX;
	float currentDistance;
	neuron* result;
	pos2d winningPosition;
	for(int i = 0 ; i < mySOM.size(); i++){
		for(int j = 0; j < mySOM[i].size(); j++){
			//calculate euclidian distance between training neuron and current neuron
			if(mySOM[i][j].leaf){
				currentDistance = absDistance(trainingNeuron,mySOM[i][j].SOMDataVector);	
			}else{
				currentDistance = absDistance(trainingNeuron,mySOM[i][j].myNestedSOM.center);	
			}
			if(currentDistance < nearestDistance){
				nearestDistance = currentDistance;
				winningPosition.x=i;
				winningPosition.y=j;
			}
		}
	}
	return winningPosition;
}

map train(map mySOM, int iterations, vector<neuron> inputVectors){
	int iterationsRemaining = iterations;
	for(; iterationsRemaining>=0; iterationsRemaining--){
		random_shuffle(inputVectors.begin(),inputVectors.end());
		for(int j =0;j < inputVectors.size();j++){
			pos2d winner = findWinningNeuron(mySOM,inputVectors[j]);
			
			mySOM = trainNeigbours(mySOM, inputVectors[j],winner,((float)iterationsRemaining/(float)iterations),((float)iterationsRemaining/(float)iterations));
		}
	}

	return mySOM;
}

void trainGHSOM(vector<neuron> inputVectors, int iterations){
	map mySOM;
}

neuron mapMQE(vector<neuron> inputVectors){
}

neuron unitMQE(vector<neuron> inputVectors){
	neuron avg = average(inputVectors);
	neuron result (avg.size(),0.0);

	for(int i = 0;i<inputVectors.size();i++){
		for(int j = 0; j<avg.size();j++){
			result[j]+=sqrt(pow(avg[j] - inputVectors[i][j],2));//abs value of the difference between avg and vector
		}
	}
	for(int j = 0; j<avg.size();j++){
		result[j]/=inputVectors.size();
	}
	return result;
}

map addSOMColumn(map mySom, int columnIndex){
	map::iterator i = mySom.begin();

	vector<region>::iterator prevCol;
	vector<region>::iterator nextCol;
	while(i != mySom.end()){

		prevCol = (*i).begin();
		nextCol = (*i).begin();

		if(columnIndex !=0)
			advance(prevCol,columnIndex-1);
		else
			advance(prevCol,columnIndex);//we essentially clone the first column to start of row

		advance(nextCol,columnIndex);
		region mid;
		mid.SOMDataVector = midpoint((*prevCol).SOMDataVector,(*nextCol).SOMDataVector);
		mid.leaf=true;
		(*i).insert(nextCol,mid);//insert places BEFORE the position, so we give nexCol as position
		i++;
	}

	return mySom;
}

map addSOMRow(map mySom, int rowIndex){
	map::iterator i = mySom.begin();//currentRow
	advance(i,rowIndex);
	vector<region>::iterator rowIter=(*i).begin();

	map::iterator nextRow = i;
	//	nextRow++;
	vector<region>::iterator nri = (*nextRow).begin();

	map::iterator previousRow=i;
	if(rowIndex > 0)
		previousRow--;//otherwise we simply clone first row to head of list

	vector<region>::iterator pri = (*previousRow).begin();

	vector<region> newRow;

	//iterate over each field in our row
	vector<region>::iterator currentField = (*i).begin();

	//must compute average of neighbors to insert a meaningful value in newRow
	while(currentField != (*i).end()){
		region NewRegion;
		NewRegion.SOMDataVector = midpoint((*pri).SOMDataVector,(*nri).SOMDataVector);
		NewRegion.leaf = true;
		newRow.push_back(NewRegion);
		currentField++;
		pri++;
		nri++;
	}
	mySom.insert(i,newRow);
	return mySom;
}

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


map tspTrainNeigbours(map mySOM, neuron trainingNeuron, pos2d winner, float neigbourhoodSize, float learningCoefficient){
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

				//region temp = mySOM[i][j];

				float influence = ((maxDistance-currentDistance)/maxDistance)*learningCoefficient;
				mySOM[i][j].SOMDataVector = attract(trainingNeuron,mySOM[i][j].SOMDataVector,influence);
			
				mySOM[i][j].SOMDataVector = repel(centerScreen,mySOM[i][j].SOMDataVector,influence);
				
				mySOM[i][j].SOMDataVector = attract(midpoint(mySOM[i][(j+1)%mySOM[0].size()].SOMDataVector,
					mySOM[0][((j-1)%mySOM[0].size())].SOMDataVector),mySOM[i][j].SOMDataVector,influence);
			}
		}
	}

	mySOM[winner.x][winner.y].SOMDataVector = attract(trainingNeuron,mySOM[winner.x][winner.y].SOMDataVector,1.0);

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
	//using abs value rather than x/y distinct prevents distortion
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

map initializeNewSOM(float meshSize,float min, float max){
	vector< vector<region> > results;

	for(float i = 0.0; i < 1.0; i+=meshSize){
		//new row
		vector<region> row;
		for(float j = 0.0; j < 1.0; j+=meshSize){
			//new column
			neuron NewNeuron;
			NewNeuron.push_back(RandomFloatInRange(min,max));
			NewNeuron.push_back(RandomFloatInRange(min,max));
			region NewRegion;
			NewRegion.SOMDataVector = NewNeuron;
			NewRegion.leaf = true;
			row.push_back(NewRegion);
		}
		results.push_back(row);
	}
	return results;
}

