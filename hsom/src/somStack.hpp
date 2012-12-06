#include <stack>

class somStack{

	private:

		//bottom of the stack is our entire data set. pushed on top of it are progressively smaller som data sets
		//and the members of the som parent class is the data we currently have loaded for visualization/manipulation
		//this is quick and easy to build, at the cost of duplicated input vector representation.
		//optimization can be done to correct this at a later time
		stack<som> somStack;

	public:
		int size(){return somStack.size();}
		//the training vectors pushed to the internal som is the set of selected BMUs in the current som
		som push(som s){
			cout<<"PUSHING_STACK!\n";

			somStack.push(s);

			//return new som context based off of previous som selection
			som subsetSOM;
			subsetSOM.init(s.getCloudIV(),s.train_max);
			return subsetSOM;
		}

		som pop(){
			if(!somStack.empty()){
				cout<<"POPPING_STACK!\n";
				//remove the top som from the stack and reinitialize our current class variables to the 
				//this is like 'zooming out' of the som data
				som lastSOM = somStack.top();
				cout<<"IV size: "<<lastSOM.trainingVectors.size()<<"\n";
				somStack.pop();
				return lastSOM;
			}else{
				cout<<"cannot pop empty stack\n";
			}
		}

};
