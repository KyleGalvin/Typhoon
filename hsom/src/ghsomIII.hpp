#include "somIII.hpp"
#include <stack>

class ghsom: public som{

	private:

		//bottom of the stack is our entire data set. pushed on top of it are progressively smaller som data sets
		//and the members of the som parent class is the data we currently have loaded for visualization/manipulation
		//this is quick and easy to build, at the cost of duplicated input vector representation.
		//optimization can be done to correct this at a later time
		stack<som*> somStack;

		void swap_context(som* context){
			//insert our newData into our parent som class' variables
			//effectively making the newData the currently viewed som.
		}

	public:

		//the training vectors pusehd to the internal som is the set of selected BMUs in the current som
		void push_stack(){
			//convert BMU set to input vector list
			set<int> group = getBMUGroup();
			vector < vector<float> > inputVectors;

			set<int>::iterator it = group.begin();

			for(;it!=group.end();it++){
				//technically, the cell returned from getCell is only LIKE our training vector...
				//this should be properly mapped to the training vector later to reduce accumulating errors
				inputVectors.push_back(getCell((*it)));
			}

			//creates another som from selected bmu group and push it onto the stack.
			//this is like 'zooming in' to the selected som data
			som* subsetSOM = new som();
			subsetSOM->init(inputVectors,train_max);
			somStack.push(subsetSOM);
		}

		void pop_stack(){
			//remove the top som from the stack and reinitialize our current class variables to the 
			//this is like 'zooming out' of the som data
			swap_context(somStack.top());
			somStack.pop();
		}

};
