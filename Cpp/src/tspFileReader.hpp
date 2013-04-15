#include <boost/tokenizer.hpp>
#include <fstream>
#include <string>

class TSPFileReader{
public:

	vector<neuron> retrieveTrainingVectors(string filename){
		string line;
		vector<neuron> myTrainingNeurons; 

		ifstream myfile(filename.c_str());
		if(myfile.is_open()){
				//skip metadata and go right to TSP city locations
				bool foundEndOfMetaData = false;
				while(!foundEndOfMetaData && myfile.good()){
					if(line.size()>0){
					}
					getline(myfile,line);
					size_t found = line.find("NODE_COORD_SECTION");
					if(found!=string::npos){
						foundEndOfMetaData = true;
					}
				}
				//TSP data is terminated with the string "EOF"
				bool foundEndOfFile = false;
				while(!foundEndOfFile && myfile.good()){
					getline(myfile,line);
					size_t found = line.find("EOF");
					if(found!=string::npos){
						foundEndOfFile = true;
					}else{

						boost::char_separator<char> tokenDivider(" ");
						//each TSP line has 3 datapoints: cityID, xLoc, yLoc. we are only interested in last 2
						boost::tokenizer<> tokens(line);

						neuron NewTrainingNeuron;
						int i = 0;
						for(boost::tokenizer<>::iterator t = tokens.begin(); t!=tokens.end();++t){
							string s = (string)t->c_str();
							if(i!=0 && s.find("\t") == string::npos){//skip 'index' value and go straight to vector data
								NewTrainingNeuron.push_back(atof(t->c_str()));
							}
							i++;
						}
						
						myTrainingNeurons.push_back(NewTrainingNeuron);
					}
				}
			myfile.close();
			
		}

		return myTrainingNeurons;
	}

};
