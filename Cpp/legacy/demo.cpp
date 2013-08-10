#include "geotypes.hpp"
#include "objectstructs.hpp"
#include "hgrid.hpp"
#include "collision.hpp"
#include "singlegrid.hpp"
#include "render.hpp"
#include <iostream>

void maptest(){
	//each cell is 10x10x10 units in dimension
	unsigned long int dimensions = 3;
	vector<Scalar> CellSize;

	for(unsigned int i=0; i<dimensions; i++){
		CellSize.push_back(10);
	}
	singlegrid M(CellSize);
	M.debug = true;

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

	//each cell is created as an object is placed, and destroyed as soon as it is emptied.
	cout<<"Grid Size Currently: "<<M.CellCount()<<" cells\n";
	M.Add(a);
	cout<<"Grid Size Currently: "<<M.CellCount()<<" cells\n";
	M.Add(b);
	cout<<"Grid Size Currently: "<<M.CellCount()<<" cells\n";
	M.Add(c);
	M.Add(c);//an object can be added twice into a grid...
	cout<<"Grid Size Currently: "<<M.CellCount()<<" cells\n";

	cout<<"\nMaking copy of grid...\n";
	singlegrid Rebuild(CellSize);//make new map. We will copy the contents of M to this
	Rebuild.Add(M.Export());//place M's exported contents into map Rebuild. This does not modify M in any way.
	cout<<"copy's Grid Size: "<<Rebuild.CellCount()<<" cells\n";
	cout<<"Finished copy. Continuing.\n\n";

	cout<<"Grid Size Currently: "<<M.CellCount()<<" cells\n";//no new cell created since object a is in the same cell as object c
	M.Remove(b);
	cout<<"Grid Size Currently: "<<M.CellCount()<<" cells\n";
	M.Remove(a);
	cout<<"Grid Size Currently: "<<M.CellCount()<<" cells\n";
	M.Remove(c);//an object in a cell more than once will have ALL instances removed.
	cout<<"Grid Size Currently: "<<M.CellCount()<<" cells\n";

	//cout<<"\nIterating:\n";
	//singlegrid::const_iterator i = Rebuild.begin();
	//while(i!=Rebuild.end()){
	//	cout<<"Cell has "<<(*i).second->BoundObjs.size()<<" Objects.\n";
	//	++i;
	//}

	cout<<"\ncopy's Grid Size Still: "<<Rebuild.CellCount()<<" cells\n";
	cout<<"clearing copy's grid...\n";
	Rebuild.Clear();
	cout<<"copy's Grid Size: "<<Rebuild.CellCount()<<" cells\n";

}

void hgridtest(){
	hgrid myHGrid;
	myHGrid.debug = true;

	boost::shared_ptr<AABB> a(new AABB);
	boost::shared_ptr<AABB> b(new AABB);
	boost::shared_ptr<AABB> c(new AABB);
	boost::shared_ptr<AABB> d(new AABB);
	boost::shared_ptr<AABB> e(new AABB);
	boost::shared_ptr<AABB> f(new AABB);//objects we will place into the 3d grid.
	//our objects are given locations
	a->Location.push_back(0);
	a->Location.push_back(0);
	a->Location.push_back(0);
	a->Dimensions.push_back(12);
	a->Dimensions.push_back(12);
	a->Dimensions.push_back(12);

	//located at (x,y,z) = (11,11,0)
	//with size 12 x 2 x 2
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

	d->Location.push_back(70);
	d->Location.push_back(70);
	d->Location.push_back(0);
	d->Dimensions.push_back(4);
	d->Dimensions.push_back(4);
	d->Dimensions.push_back(4);

	e->Location.push_back(70);
	e->Location.push_back(70);
	e->Location.push_back(0);
	e->Dimensions.push_back(40);
	e->Dimensions.push_back(40);
	e->Dimensions.push_back(40);

	f->Location.push_back(0);
	f->Location.push_back(0);
	f->Location.push_back(0);
	f->Dimensions.push_back(1);
	f->Dimensions.push_back(1);
	f->Dimensions.push_back(1);

	cout<<myHGrid.CellCount()<<" cells in "<<myHGrid.GridCount()<<" grids.\n";
	myHGrid.Add(a);
	cout<<myHGrid.CellCount()<<" cells in "<<myHGrid.GridCount()<<" grids.\n";
	myHGrid.Add(b);
	cout<<myHGrid.CellCount()<<" cells in "<<myHGrid.GridCount()<<" grids.\n";
	myHGrid.Add(b);
	cout<<myHGrid.CellCount()<<" cells in "<<myHGrid.GridCount()<<" grids.\n";
	myHGrid.Add(c);
	cout<<myHGrid.CellCount()<<" cells in "<<myHGrid.GridCount()<<" grids.\n";
	myHGrid.Add(d);
	cout<<myHGrid.CellCount()<<" cells in "<<myHGrid.GridCount()<<" grids.\n";
	myHGrid.Add(e);
	cout<<myHGrid.CellCount()<<" cells in "<<myHGrid.GridCount()<<" grids.\n";
	myHGrid.Add(f);
	cout<<myHGrid.CellCount()<<" cells in "<<myHGrid.GridCount()<<" grids.\n";

	hgrid myHGridTwo;
	cout<<"Copying!\n";
	myHGridTwo.Add(myHGrid.Export());
	cout<<"G1: "<<myHGrid.CellCount()<<" cells in "<<myHGrid.GridCount()<<" grids.\n";
	cout<<"G2: "<<myHGridTwo.CellCount()<<" cells in "<<myHGridTwo.GridCount()<<" grids.\n";

	myHGrid.Clear();

	cout<<"G1: "<<myHGrid.CellCount()<<" cells in "<<myHGrid.GridCount()<<" grids.\n";
	cout<<"G2: "<<myHGridTwo.CellCount()<<" cells in "<<myHGridTwo.GridCount()<<" grids.\n";

	//cout<<"\nIterating:\n";
	//hgrid<areagrid>::const_iterator i = myHGridTwo.begin();
	//cout<<"in demo\n";
	//hgrid<areagrid>::const_iterator y = myHGridTwo.end(); 
	//cout<<"end clear\n";
	//cout<<(i == y)<<"trah lah lahhhh!\n";
	//while(i!=myHGridTwo.end()){
	//	cout<<"in demo while\n";
	//	cout<<"Cell has "<<(*i).second->BoundObjs.size()<<" Objects.\n";
	//	++i;
	//}

}

void collisiontest(){
	CollisionEngine CTest;
	CTest.debug = true;
	AABB a,b;

	//our objects are given locations
	a.Location.push_back(0);
	a.Location.push_back(0);
	a.Location.push_back(0);
	a.Dimensions.push_back(12);
	a.Dimensions.push_back(12);
	a.Dimensions.push_back(12);

	//located at (x,y,z) = (11,11,0)
	//with size 12 x 2 x 2
	b.Location.push_back(13);
	b.Location.push_back(0);
	b.Location.push_back(0);
	b.Dimensions.push_back(12);
	b.Dimensions.push_back(2);
	b.Dimensions.push_back(2);

	//&a and &b are implicitly typecast from AABB* to Object*
	bool collision = CTest.TestObjects(&a,&b);
	if(collision){
		cout<<"A and B overlap\n";
	}else{
		cout<<"A and B do not overlap\n";
	}
}

void AABBTest(){
	typedef vector<Line> LineList;
	typedef boost::shared_ptr<LineList> PtrLineList;
	AABB BBox;
	BBox.debug = true;
	BBox.Location.push_back(0);
	BBox.Location.push_back(0);
	BBox.Location.push_back(0);
	BBox.Location.push_back(0);
	BBox.Dimensions.push_back(1);
	BBox.Dimensions.push_back(1);
	BBox.Dimensions.push_back(1);
	BBox.Dimensions.push_back(1);
	PtrLineList Result = BBox.GetShape();

	LineList::iterator Li = Result->begin();
	Coordinate::iterator Ci;
	cout<<"returned!\n";
}

void singleitertest(){
	typedef Object Obj;
	typedef boost::shared_ptr<Obj> PtrObj;
	typedef vector<PtrObj> ListPtrObj;
	typedef set<PtrObj> SetPtrObj;

	//grid cell size
	Coordinate Dim;
	Dim.push_back(10);
	Dim.push_back(10);

	singlegrid myGrid(Dim);

	//create a buncha objects
	boost::shared_ptr<AABB> a(new AABB);
	boost::shared_ptr<AABB> b(new AABB);
	boost::shared_ptr<AABB> c(new AABB);
	boost::shared_ptr<AABB> d(new AABB);
	boost::shared_ptr<AABB> e(new AABB);
	boost::shared_ptr<AABB> f(new AABB);
	boost::shared_ptr<AABB> g(new AABB);

	//A spans 2 cells
	a->Dimensions.push_back(8);
	a->Dimensions.push_back(12);

	a->Location.push_back(1);
	a->Location.push_back(1);

	b->Dimensions.push_back(8);
	b->Dimensions.push_back(8);

	b->Location.push_back(1);
	b->Location.push_back(11);

	myGrid.Add(a);
	myGrid.Add(b);
	singlegrid::const_iterator i = myGrid.begin();
	ListPtrObj::iterator j;
	while(i != myGrid.end()){
		j=(*i)->BoundObjs.begin();
		while(j !=(*i)->BoundObjs.end()){
					cout<<"grid "<<(*j)->Location[0]<<","<< (*j)->Location[1]<<"\n";
		++j;
		}
		++i;

	}
	cout<<"TADAH\n";

	SetPtrObj test = myGrid.GetNeighbours(b);
	SetPtrObj::iterator k = test.begin();
	while(k!= test.end()){
		cout<<"Sharing with b:"<<(*k)->Location[0]<<","<< (*k)->Location[1]<<"\n";
		++k;
	}
}

int main(){
	singleitertest();
	maptest();
	hgridtest();
	//collisiontest();
	//AABBTest();
	return 0;
}
