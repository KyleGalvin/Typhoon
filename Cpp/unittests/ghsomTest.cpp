
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE growingSomTest
#include <boost/test/unit_test.hpp>
#include "../src/ghsom.hpp"
#include "../src/tspFileReader.hpp"

bool floatComp(float f1, float f2){

	float precision = 0.000001;

	if (((f1 - precision) < f2) && ((f1 + precision) > f2))
		return true;

	return false;
}

BOOST_AUTO_TEST_CASE( testgrowingsom ){

	TSPFileReader trainingVectorFactory;
	vector<neuron> TrainingNeurons;
	vector< vector<neuron> > mySOM;

	//create 2d 20x20 mesh of som vectors with randomized data between 0.0f and 1.0f		
	mySOM =  initializeNewSOM(0.05,0.0,1.0);
	
	BOOST_CHECK(mySOM.size() == 20);
	BOOST_REQUIRE(mySOM.size() > 0);
	
	vector<neuron> mySomRow1 = mySOM[0];
	vector<neuron> mySomRow2 = mySOM[10];
	vector<neuron> mySomRow3 = mySOM[19];


	BOOST_CHECK(mySomRow1.size() == 20);
	BOOST_REQUIRE(mySomRow1.size() > 0);
	BOOST_CHECK(mySomRow2.size() == 20);
	BOOST_REQUIRE(mySomRow2.size() > 0);
	BOOST_CHECK(mySomRow3.size() == 20);
	BOOST_REQUIRE(mySomRow3.size() > 0);

	//begin growing the som
	mySOM = addSOMColumn(mySOM,10);	

	mySomRow1 = mySOM[0];
	mySomRow2 = mySOM[10];
	mySomRow3 = mySOM[19];

	BOOST_REQUIRE(mySomRow1.size() ==21);
	BOOST_REQUIRE(mySomRow2.size() ==21);
	BOOST_REQUIRE(mySomRow3.size() ==21);

	
	//check that the new column data is the average of the neighboring values
	int newcolumn = 10;
	for(int i = 0; i < mySOM.size(); i++){

		mySomRow1 = mySOM[i];
		BOOST_CHECK( floatComp(mySomRow1[newcolumn][0], ( mySomRow1[newcolumn-1][0] + mySomRow1[newcolumn+1][0] ) / 2.0) );
		BOOST_CHECK( floatComp(mySomRow1[newcolumn][1], ( mySomRow1[newcolumn-1][1] + mySomRow1[newcolumn+1][1] ) / 2.0) );
	}

	//continue growing the som
	mySOM = addSOMRow(mySOM,10);

	BOOST_REQUIRE(mySOM.size() == 21);
	
	//check that the new row data is the average of the neighboring values
	int newrow = 10;
	mySomRow1 = mySOM[newrow];//newest row we just placed in
	vector<neuron> mySomRowPrev = mySOM[newrow-1];
	vector<neuron> mySomRowNext = mySOM[newrow+1];
	for(int i = 0;i<mySomRow1.size();i++){
		//array [i][0] and [i][1] index maps to x/y values of the 2d som data vectors
		BOOST_CHECK( floatComp(mySomRow1[i][0], ( mySomRowPrev[i][0] + mySomRowNext[i][0] ) / 2.0) );
		BOOST_CHECK( floatComp(mySomRow1[i][1], ( mySomRowPrev[i][1] + mySomRowNext[i][1] ) / 2.0) );
	}

	//test adding to first row/column (as we usually depend on the row/column 'previous' to our index to get average)
	//note: this shouldn't be strictly needed, as som growth is defined 
	//as adding a row/column BETWEEN EXISTING ELEMENTS rather than at the start/end of an array
	mySOM = addSOMRow(mySOM,0);
	mySOM = addSOMColumn(mySOM,0);

	//test column zero growth
	for(int i = 0;i<mySOM.size();i++){
		mySomRow1 = mySOM[i];
		BOOST_CHECK( floatComp(mySomRow1[0][0],mySomRow1[1][0]));
		BOOST_CHECK( floatComp(mySomRow1[0][1],mySomRow1[1][1]));
	}

	//test row zero growth
	mySomRow1 = mySOM[0];
	mySomRow2 = mySOM[1];
	for(int i = 0;i<mySomRow1.size();i++){
		BOOST_CHECK(mySomRow1[i][0] == mySomRow2[i][0]);
		BOOST_CHECK(mySomRow1[i][1] == mySomRow2[i][1]);
	}

}

BOOST_AUTO_TEST_CASE( testunitmqe ){
	
	TSPFileReader trainingVectorFactory;
	vector<neuron> TrainingNeurons;
	vector< vector<neuron> > mySOM;


//	TrainingNeurons = createHierarchalMeshData(1/meshSize,1/meshSize);
	TrainingNeurons = createMeshData(1/meshSize,1/meshSize);
	neuron mqe = unitMQE(TrainingNeurons);
	for(int i = 0; i < mqe.size(); i++){
		cout<<mqe[i]<<"\n";
	}
}
