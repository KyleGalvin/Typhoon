
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
//#define BOOST_TEST_MODULE singlegrid
#include <boost/test/unit_test.hpp>
#include "singlegrid.hpp"

BOOST_AUTO_TEST_CASE( singlegridtest ){

	unsigned long int dimensions = 3;
	vector<Scalar> cellSize;

	for(unsigned int i=0; i<dimensions; i++){
		cellSize.push_back(10);//each cell in the grid is a 10x10x10 cube
	}
	BOOST_CHECK( cellSize.size() == dimensions );//has our vector been filled appropriately?
	
	singlegrid grid(cellSize);//create grid with specified size and specified # of dimesions
	BOOST_CHECK( grid.CellCount() == 0 );//is our grid initialized and empty?
	
	for(unsigned int i=0; i<dimensions; i++){
		//does our grid cell size mirror the dimensions requested of it?
		BOOST_CHECK( grid.GetCellSize(i) == cellSize[i] );
	}	
	
	boost::shared_ptr<AABB> a (new AABB);
	boost::shared_ptr<AABB> b (new AABB);
	boost::shared_ptr<AABB> c (new AABB);//objects we will place into the 3d grid.
	
	//our objects are given locations
	a->Location.push_back(0);
	a->Location.push_back(0);
	a->Location.push_back(0);
	a->Dimensions.push_back(12);
	a->Dimensions.push_back(12);
	a->Dimensions.push_back(12);

	b->Location.push_back(11);
	b->Location.push_back(11);
	b->Location.push_back(0);
	b->Dimensions.push_back(12);
	b->Dimensions.push_back(2);
	b->Dimensions.push_back(2);

	c->Location.push_back(70);
	c->Location.push_back(70);
	c->Location.push_back(0);
	c->Dimensions.push_back(2);
	c->Dimensions.push_back(2);
	c->Dimensions.push_back(2);
	
	/*(a) is larger than the grid cell in every dimension (but smaller than twice as large),
	which makes it take up 2 cells in all dimensions. 2^3=8 */
	grid.Add(a);
	BOOST_CHECK( grid.CellCount() == 8 );
	/*(b) has one dimension larger than the grid cell, so it takes up 2 cells total
	  one of these cells overlaps with a cell (a) is in, so this cell doesnt need
	  to be created. 1x1x2 = 2 new cells, subtract the cell that exists for added 1 cell.*/
	grid.Add(b);
	BOOST_CHECK( grid.CellCount() == 9 );
	/*(c) is small and far away from (a) and (b). It should take up one cell
	  of space which will be automatically created/initialized.*/
	grid.Add(c);
	BOOST_CHECK( grid.CellCount() == 10 );

	/*adding an identical object twice should not add cells, as there is no new occupied space
	  as a note, attempting to DELETE one instance of c will result in the deletion of both.*/
	grid.Add(c);
	BOOST_CHECK( grid.CellCount() == 10 );
	
	//place M's exported contents into new map 'rebuild'. This does not modify M in any way.
	singlegrid rebuild(cellSize);
	rebuild.Add(grid.Export());
	BOOST_CHECK( grid.CellCount() == rebuild.CellCount() );

	/*Here we test the removal of objects and the automatic cleanup/removal of empty cells.
	  Even though (c) has been inserted twice, both are removed in this same call.*/
	rebuild.Remove(c);
	BOOST_CHECK( rebuild.CellCount() == 9 );

	//(a) covers 8 cells, yet one of these shares a cell with (b). 7 empty cells removed
	rebuild.Remove(a);
	BOOST_CHECK( rebuild.CellCount() == 2 );

	//Remove all remaining objects, effectively removing (b) and the 2 cells it resides in.
	rebuild.Clear();
	BOOST_CHECK( rebuild.CellCount() == 0 );

	//A more appropriate case of the above
	BOOST_CHECK( grid.CellCount() == 10 );
	grid.Clear();
	BOOST_CHECK( grid.CellCount() == 0 );
}


BOOST_AUTO_TEST_CASE( singlegrid_iterator ){

	UNSIgned long int dimensions = 3;
	vector<Scalar> cellSize;

	for(unsigned int i=0; i<dimensions; i++){
		cellSize.push_back(10);//each cell in the grid is a 10x10x10 cube
	}
	
	singlegrid grid(cellSize);//create grid with specified size and specified # of dimesions
	
	boost::shared_ptr<AABB> a (new AABB);
	boost::shared_ptr<AABB> b (new AABB);
	boost::shared_ptr<AABB> c (new AABB);//objects we will place into the 3d grid.
	
	//our objects are given locations
	a->Location.push_back(0);
	a->Location.push_back(0);
	a->Location.push_back(0);
	a->Dimensions.push_back(12);
	a->Dimensions.push_back(12);
	a->Dimensions.push_back(12);

	b->Location.push_back(11);
	b->Location.push_back(11);
	b->Location.push_back(0);
	b->Dimensions.push_back(12);
	b->Dimensions.push_back(2);
	b->Dimensions.push_back(2);

	c->Location.push_back(70);
	c->Location.push_back(70);
	c->Location.push_back(0);
	c->Dimensions.push_back(2);
	c->Dimensions.push_back(2);
	c->Dimensions.push_back(2);
	
	singlegrid::const_iterator i = grid.begin();
	ListPtrObj::iterator j;
	while(i != grid.end()){
		j=(*i)->BoundObjs.begin();
		while(j !=(*i)->BoundObjs.end()){
					cout<<"grid "<<(*j)->Location[0]<<","<< (*j)->Location[1]<<"\n";
		++j;
		}
		++i;

	}
}
