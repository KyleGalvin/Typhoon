#include "float.h"
#include "math.h"
#include "vectorMath.hpp"
#include <algorithm>
#include <queue>
#include <set>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

using namespace std;

typedef vector<float> neuron;

float randomFloatInRange(float startRange, float endRange){
	float random = ((float) rand()) / (float) RAND_MAX;
	float range = endRange-startRange;
	return (random*range) + startRange;
}

class som{

	//protected so inherited functions have deep access.
	protected:
		//the MQE of each cell. There is a one-to-one sequential pairing of cells and cellsMQE
		vector < int > BMUCount;

		typedef boost::shared_mutex Lock;
		typedef boost::unique_lock<Lock> WriteLock;
		typedef boost::shared_lock<Lock> ReadLock;

		//provides a read/write lock for our cells array
		Lock r_w_lock;

		//cube dimensions
		int h;
		int w;
		int d;
		
		//data to train the SOM to
		vector<neuron> trainingVectors;
		queue<neuron> trainingBuffer;

		//for step-by-step training (allowing for rendering between)
		int train_max;
		int train_current;

		//the SOM data structure
		vector < neuron > cells;

		//mouse picking index
		int selectedCellSingle;

		//cloud selection group
		vector < int > selectedCellGroup;

		//cloud selection group
		set < int > selectedBMUGroup;

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

		//this can help us decide which cells fit a group query by giving us a metric of similarity to a given inputVector
		//tolerance is described here as 'MQE' percentage (0.0 to 1.0) of our SOM data range.
		vector<float> getTolerance(float MQE){
			//get the domain of our SOM space;
			//hard coded for color data here since dynamic calculation costs h*w*d*inputVector.size() iterations
			vector<float> minVal;
			minVal.push_back(0);
			minVal.push_back(0);
			minVal.push_back(0);

			vector<float> maxVal;
			maxVal.push_back(255);
			maxVal.push_back(255);
			maxVal.push_back(255);

			vector<float> range = delta(maxVal,minVal);
			range = scale(MQE,range);
			cout<<"range:"<<range[0]<<" "<<range[1]<<" "<<range[2]<<"\n";
			return range;

		}

	public:

		//read-only access functions
		int get_h(){
			//take read-access
			ReadLock rLock(r_w_lock);

			return h;
		}
		int get_w(){
			//take read-access
			ReadLock rLock(r_w_lock);
			return w;
		}
		int get_d(){
			//take read-access
			ReadLock rLock(r_w_lock);
			return d;
		}
		vector<float> getCell(int i){
			//take read-access
			ReadLock rLock(r_w_lock);
			return cells[i];
		}
		int getSelectedCell(){
			//take read-access
			ReadLock rLock(r_w_lock);
			return selectedCellSingle;
		}
		vector < int > getCellGroup(){
			//take read-access
			ReadLock rLock(r_w_lock);
			return selectedCellGroup;
		}
		set < int > getBMUGroup(){
			//take read-access
			ReadLock rLock(r_w_lock);
			return selectedBMUGroup;
		}
		int getIterations(){
			//take read-access
			ReadLock rLock(r_w_lock);
			return train_current;
		}
		void clearCellGroup(){
			//lock read/write access
			WriteLock wLock(r_w_lock);
			selectedCellGroup.clear();
		}

		void setSelectedCell(int cell){
			//lock read/write access
			WriteLock wLock(r_w_lock);
			selectedCellSingle = cell;
		}
		int getBMUCount(int cell){
			//take read-access
			ReadLock rLock(r_w_lock);
			return BMUCount[cell];
		}

		void init(vector< neuron > t_v, int x_res, int y_res, int z_res, int tIterations){
			//lock read/write access
			WriteLock wLock(r_w_lock);

			//no cell initially selected
			selectedCellSingle = -1;

			//save a copy of the vectors we are to train to
			trainingVectors = t_v;

			BMUCount.assign(trainingVectors.size(),0);//make a list of zero values.

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
			cout<<"init complete\n";
		}
		
		//estimate an appropriate SOM size based on the number of input vectors
		//we assume a 3D som in a cube structure to be appropriate
		void init(vector< neuron > t_v, int tIterations){
			//cube root to evenly divide input vectors along square 3d structure
			//we round this value up by adding one before dropping decimal in integer conversion
			int size = pow(t_v.size(),1.0/3.0) + 1;
			init(t_v,size,size,size,tIterations);
		}

		//take a single random training vector and impress it on the SOM
		//training vectors are arranged in the queue (trainingBuffer) to ensure even distribution of training impressions
		void trainStep(){
			//lock read/write access
			WriteLock wLock(r_w_lock);
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
			//technically we should use a read lock. Practically, no current use-case has more than one thread access trainingBuffer
			while(trainingBuffer.size()>0){
				trainStep();
			}
			fillTrainingBuffer();
		}


		void trainAll(){
			//technically we should use a read lock. Practically, no current use-case has more than one thread access train_current
			while(train_current > 0){
				trainPass();
			}
		}

		

		//get the set of SOM vectors that are less than MQE percent different from inputVector
		//this is a way to select all our som data in a region,
		//which includes a cloud of SOM data that is (fuzzy logic) 'like' our inputVector
		vector< vector< float > > getCloudAll(vector<float> inputVector, float MQE){
			//The tolerance describes
			//an acceptable amount of deviation from the inputVector
			//to help us define our cloud.
			vector<float> tolerance = getTolerance(MQE);

			//find all SOM data that is 'within tolerance' of our input vector
			vector< vector< float > > resultData;
			vector< int > resultIndex;
			for(int i = 0; i<h*w*d; i++){
				//if the cell is near enough our input vector (closer than the length of our tolerance vector)
				if(vLength(delta(inputVector,getCell(i))) < vLength(tolerance)){
					//push to our winning neuron list
					resultData.push_back(getCell(i));
					resultIndex.push_back(i);
				}
				
			}

			//lock read/write access
			WriteLock wLock(r_w_lock);

			//save the last known selection group in our model for easy reference while rendering
			selectedCellGroup = resultIndex;

			//give the user the detailed version of the query
			return resultData;
		}

		//get the set of trainingVectors that are less than MQE percent different from inputVector.
		//sort by proximity to inputVector.
		//this is a way to select the 'real data' in a region, 
		//which best represents our training procedure vectors
		void getCloudBMU(vector<float> inputVector, float MQE){
			//I chose a set to cull duplicates
			//first we find the set of all BMU's to our training inputs
			set<int> BMUIndexSet;


			for(int i = 0; i < trainingVectors.size();i++){
				//get the set of BMU's
				BMUIndexSet.insert(findBMU(trainingVectors[i]));
				//while we are here, we also count the number of training vectors that map to a given cell
			//	BMUCount[findBMU(trainingVectors[i])]++;//increment our index bmu count
				
			}
			//next we make a set of all selected/hilighted cells in our cloud
			//convert the vector list to a proper set
			set<int> SelectedSet;
			for(int i = 0; i <selectedCellGroup.size();i++){
				SelectedSet.insert(selectedCellGroup[i]);
			}

			//std lib set intersection algorithm. Intersection stored in results variable
			set<int> results;
			set_intersection(BMUIndexSet.begin(), BMUIndexSet.end(),SelectedSet.begin(), SelectedSet.end(), 
				insert_iterator< set<int> >(results, results.begin()));

			//lock read/write access
			WriteLock wLock(r_w_lock);

			selectedBMUGroup = results;
		}

};

