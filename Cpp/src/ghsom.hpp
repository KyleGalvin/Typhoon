#include <vector>
#include <fstream>
#include <string>
#include "float.h"
#include "math.h"
#include "vectorMath.hpp"
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

typedef vector<float> neuron;
class region;

typedef vector< vector<region> > som; //2d neighborhood

class region{
	typedef vector<float> neuron;

	struct pos2d{
		int x;
		int y;
	};

	public:
		int TrainingIterations;//Number of iterative passes to use while training
		neuron mqe;//mean quantization error of our single SOMDataVector
		neuron MQE;//mean quantization error of our nested SOM data
		neuron center;//focus of the map (average of input vectors)
		vector<neuron> inputVectors;//input vectors to train with
		bool leaf;//helps decide if this structure is a final node of a link in the recursive chain
		float tau_1;
		float tau_2;
		
		//leaf structure
		neuron SOMDataVector;

		//recursive structure
		som myNestedSOM;

		region(neuron bootstrapNeuron, int Iterations,float t1, float t2){
			TrainingIterations = Iterations;
			tau_1 = t1;
			tau_2 = t2;

			//we don't recurse in the constructor, or else we'd be initializing forever
			leaf=true;
			
			//boostrapNeuron tells us the dimensionality of our data vectors
			//already pre-randomized (should be at least initialized and with a non-zero size)
			SOMDataVector = bootstrapNeuron;

		}

		neuron unitCenter(vector<neuron> inputVectors){
			return average(inputVectors);
		}

		void loadInputVectors(vector<neuron> input){
			inputVectors = input;
			center = unitCenter(inputVectors);	
			calculateUnitMQE(inputVectors);
		}

		//Must have inputVectors initialized before calling this. use loadInputVectors to do so
		void train(){

			//if we are still a leaf node, break the data down into our sub-som
			if(myNestedSOM.size() == 0){
				partition();
			}
	
			//start us off into the loop
			MQE = mqe;
			cout<<"Training!\n";
			while(vLength(MQE) >= tau_1 * vLength(mqe)){
				cout<<"give it a whirl!\n";
				//train the entire sub-som to our entire set of input vectors
				for(int i = TrainingIterations; i > 0; i--){
					random_shuffle(inputVectors.begin(),inputVectors.end());
					for(int j =0;j < inputVectors.size();j++){
						pos2d winner = findWinningNeuron(myNestedSOM,inputVectors[j]);
						myNestedSOM = trainNeigbours(myNestedSOM, inputVectors[j],winner,
								((float)i/(float)TrainingIterations),
								((float)i/(float)TrainingIterations));
					}
					
				}
				cout<<"done training round\n";
				partitionIV();//(re)partition input vectors into lower level som nodes
				cout<<"calculating SOM MQE\n";
				calculateSomMQE();
				cout<<"done calculating SOM Mqe\n";
				growthManager();
				cout<<"Som node count:"<<(myNestedSOM.size()*myNestedSOM[0].size())<<"\n";
			}
			cout<<"done horizontal!\n";
			for(int i = 0; i<myNestedSOM.size();i++){
				for(int j = 0; j<myNestedSOM[i].size();j++){
					//base case of recursive feature is when this condition is never met and no more training occurs
					if(vLength(myNestedSOM[i][j].mqe) > tau_2 * vLength(mqe)){
						myNestedSOM[i][j].train();
					}
				}
			}
		}

	private:
		void partition(){
			//SOMDataStructure no longer relevant; myNestedSOM kicks in
			leaf = false;

			//create 2x2 grid  'center.size()' dimensional som structure randomly scaled between 0 and 1
			myNestedSOM =  initializeNewSOM(0.5,0.0,1.0,center.size());

		}

		void partitionIV(){
			vector< vector< vector< neuron > > >  divisionList;

			//make first two arrays size of myNestedSOM.
			//the third vector array is for a list of associated neurons.
			divisionList.resize(myNestedSOM.size());
			for(int i = 0 ; i  < myNestedSOM.size(); i++){
				divisionList[i].resize(myNestedSOM[i].size());
			}

			cout<<"ready to divide data:\n";

			//divide input into groups for each low-level som neuron
			for(int i = 0; i < inputVectors.size(); i++){
				pos2d position = findWinningNeuron(myNestedSOM,inputVectors[i]);
				divisionList[position.x][position.y].push_back(inputVectors[i]);
			}	

			cout<<"data divided\n";

			//place input into each low-level som neuron
			for(int i = 0; i < myNestedSOM.size(); i++){
				for(int j = 0;j<myNestedSOM[0].size();j++){
					cout<<"loading input vectors into som. divisionList Size:"<<divisionList[i][j].size()<<"\n";
					if(divisionList[i][j].size() > 0){
						myNestedSOM[i][j].loadInputVectors(divisionList[i][j]);
					}else{
						neuron zero(inputVectors[0].size(),0.0);
						myNestedSOM[i][j].mqe = zero;
					}
				}
			}	

			cout<<"som now modified\n";
		}

		float RandomFloatInRange(float startRange, float endRange){
			float random = ((float) rand()) / (float) RAND_MAX;
			float range = endRange-startRange;
			return (random*range) + startRange;
		}

		som initializeNewSOM(float meshSize,float min, float max, int dimensions){
			vector< vector<region> > results;

			for(float i = 0.0; i < 1.0; i+=meshSize){
				//new row
				vector<region> row;
				for(float j = 0.0; j < 1.0; j+=meshSize){
					//new column
					neuron NewNeuron;
					for(int n=0; n<dimensions; n++){
						NewNeuron.push_back(RandomFloatInRange(min,max));
					}
					region NewRegion(NewNeuron,TrainingIterations,tau_1,tau_2);
					NewRegion.SOMDataVector = NewNeuron;
					row.push_back(NewRegion);
				}
				results.push_back(row);
			}
			return results;
		}

		void calculateSomMQE(){

			vector<neuron> mqeList;

			for(int i=0 ;i < myNestedSOM.size(); i++){
				for(int j=0 ;j < myNestedSOM[0].size(); j++){
					//translate is vector addition. Imagine 'result += value'
					mqeList.push_back( myNestedSOM[i][j].mqe );
					cout<<"size:"<<myNestedSOM[i][j].mqe.size()<<"\n";

				}
			}
			cout<<"list built\n";
			MQE=average(mqeList);

			cout<<"averaged!\n";
		}

		void calculateUnitMQE(vector<neuron> inputVectors){
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

			mqe = result;
		}

		void growthManager(){

			//find highest mqe node
			pos2d highestmqeNode;
			highestmqeNode.x = 0;
			highestmqeNode.y = 0;
			float highestmqe = vLength(myNestedSOM[0][0].mqe);

			for(int i= 0; i < myNestedSOM.size();i++){
				for(int j=0; j < myNestedSOM[0].size();j++){
					if(vLength(myNestedSOM[i][j].mqe) > highestmqe){
						highestmqe = vLength(myNestedSOM[i][j].mqe);
						highestmqeNode.x = i;
						highestmqeNode.y = j;
					}				
				}
			}
			cout <<"Highest Node found\n";
			//find most dissimilar neighbor of above node. start with an identical node for easy contrast
			float MaxAbsDistance = 0.0;
			pos2d Result;
			float currentDistance = 0.0;
			bool isarow = true; 
			if(highestmqeNode.x >0){
				currentDistance = absDistance(myNestedSOM[highestmqeNode.x][highestmqeNode.y].SOMDataVector,
					myNestedSOM[highestmqeNode.x-1][highestmqeNode.y].SOMDataVector);
				if(MaxAbsDistance < currentDistance){
					Result = highestmqeNode;
					Result.x--;
					isarow = false;
					MaxAbsDistance = currentDistance;
				}
			}
			if(highestmqeNode.x < (myNestedSOM.size()-1)){
				currentDistance = absDistance(myNestedSOM[highestmqeNode.x][highestmqeNode.y].SOMDataVector,
					myNestedSOM[highestmqeNode.x+1][highestmqeNode.y].SOMDataVector);
				if(MaxAbsDistance < currentDistance){
					Result = highestmqeNode;
					Result.x++;
					isarow = false;
					MaxAbsDistance = currentDistance;
				}
			}
			if(highestmqeNode.y >0){
				currentDistance = absDistance(myNestedSOM[highestmqeNode.x][highestmqeNode.y].SOMDataVector,
					myNestedSOM[highestmqeNode.x][highestmqeNode.y-1].SOMDataVector);
				if(MaxAbsDistance < currentDistance){
					Result = highestmqeNode;
					Result.y--;
					isarow = true;
					MaxAbsDistance = currentDistance;
				}
			}
			if(highestmqeNode.y < (myNestedSOM[0].size()-1)){
				currentDistance = absDistance(myNestedSOM[highestmqeNode.x][highestmqeNode.y].SOMDataVector,
					myNestedSOM[highestmqeNode.x][highestmqeNode.y+1].SOMDataVector);
				if(MaxAbsDistance < currentDistance){
					Result = highestmqeNode;
					Result.y++;
					isarow = true;
					MaxAbsDistance = currentDistance;
				}
			}
			
			cout <<"most dissimilar Node found\n";
			//add nodes between two
			if(isarow){
				if(Result.y > highestmqeNode.y){
					cout<<"add before row "<<Result.y<<"\n";
					addSOMRow(Result.y);
				}else{
					cout<<"add before row "<<highestmqeNode.y<<"\n";
					addSOMRow(highestmqeNode.y);
				}
			}else{
				if(Result.x > highestmqeNode.x){
					cout<<"add before column "<<Result.x<<"\n";
					addSOMColumn(Result.x);
				}else{
					cout<<"add before column "<<highestmqeNode.x<<"\n";
					addSOMColumn(highestmqeNode.x);
				}
			}
			cout<<"column/row added. Count:"<<(myNestedSOM.size() * myNestedSOM[0].size())<<"\n";
		}

		void addSOMColumn(int columnIndex){
			cout<<"column\n";
			som::iterator i = myNestedSOM.begin();

			vector<region>::iterator prevCol;
			vector<region>::iterator nextCol;
			while(i != myNestedSOM.end()){

				prevCol = (*i).begin();
				nextCol = (*i).begin();

				if(columnIndex !=0)
					advance(prevCol,columnIndex-1);
				else
					advance(prevCol,columnIndex);//we essentially clone the first column to start of row

				advance(nextCol,columnIndex);
				region mid(midpoint((*prevCol).SOMDataVector,(*nextCol).SOMDataVector), TrainingIterations,tau_1, tau_2);
	
				cout<<"column iterating...Region Size:"<<mid.SOMDataVector.size()<<"Data:"<<mid.SOMDataVector[2]<<"\n";
				(*i).insert(nextCol,mid);//insert places BEFORE the position, so we give nexCol as position
				i++;
			}
			cout<<"cleaning up\n";
		}

		void addSOMRow(int rowIndex){
			cout<<"row\n";
			som::iterator i = myNestedSOM.begin();//currentRow
			advance(i,rowIndex);
			vector<region>::iterator rowIter=(*i).begin();

			som::iterator nextRow = i;
			//	nextRow++;
			vector<region>::iterator nri = (*nextRow).begin();

			som::iterator previousRow=i;
			if(rowIndex > 0){
				previousRow--;//otherwise we simply clone first row to head of list
			}

			vector<region>::iterator pri = (*previousRow).begin();

			vector<region> newRow;

			//iterate over each field in our row
			vector<region>::iterator currentField = (*i).begin();
			cout<<"ready to row-insert\n";
			//must compute average of neighbors to insert a meaningful value in newRow
			while(currentField != (*i).end()){
				region NewRegion(midpoint((*pri).SOMDataVector,(*nri).SOMDataVector), TrainingIterations,tau_1, tau_2);
				cout<<"Region Size:"<<NewRegion.SOMDataVector.size()<<"\n";
				newRow.push_back(NewRegion);
				currentField++;
				pri++;
				nri++;
			}
			cout<<"averaged. newRowsize:"<<newRow.size()<<"\n";
			myNestedSOM.insert(i,newRow);
			cout<<"placed. SOM Row count:"<<myNestedSOM.size()<<" \n";

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
						mySOM[i][j].SOMDataVector = attract(trainingNeuron,mySOM[i][j].SOMDataVector,influence);
					}
				}
			}

			return mySOM;	
		}

		pos2d findWinningNeuron(som mySOM, neuron trainingNeuron){
			//start far away so our neural network is clearly nearer.
			float nearestDistance= FLT_MAX;
			float currentDistance;
			neuron* result;
			pos2d winningPosition;
			for(int i = 0 ; i < mySOM.size(); i++){
				for(int j = 0; j < mySOM[i].size(); j++){
					//calculate euclidian distance between training neuron and current neuron
					currentDistance = absDistance(trainingNeuron,mySOM[i][j].SOMDataVector);	
					if(currentDistance < nearestDistance){
						nearestDistance = currentDistance;
						winningPosition.x=i;
						winningPosition.y=j;
					}
				}
			}
			return winningPosition;
		}

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

