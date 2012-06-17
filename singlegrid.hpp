#ifndef __SINGLEGRID_HPP__
#define __SINGLEGRID_HPP__

#include <map>
#include <string>
#include <limits>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <memory>
#include <iterator>
#include "grid.hpp"
using namespace std;


//template class to convert nearly any datatype to a string.
//Specifically speaking, we use this to take numeric coordinates and
//calculate a unique string hash
template <class T>
inline string ToString(const T& data){
	stringstream ss;
	ss << data;
	return ss.str();
}

/**This grid map places objects into any cell their footprint overlaps.*/
class singlegrid: public grid{

	protected:
		///each map cell is a vector of object pointers
		map<string, PtrObj > Cells;

		/**the location of this grid in an external coordinate system. This is
		//unrelated to the grid's internal coordinate system.*/
		Coordinate gridLocation;

		///the dimensions of each cell in the map
		Size CellSize;

		/**	Encodes cell coordinates in a unique string allowing easy access to Cells.*/
		string GetKey(Coordinate Location){

			if(Location.size() != CellSize.size()){
				cout<<"ERROR! Dimension vector improper length. Aborting";
				exit(1);
			}

			string Result;
			string Segment;

			//the hash string has a fixed length to prevent collisions
			string SMax = ToString(numeric_limits<Scalar>::max());

			Coordinate::iterator it = Location.begin();

			//go through all dimensions of our coordinates and makes a unique string from them
			while(it!=Location.end()){
				Segment = ToString(*it);
				while(Segment.length()<SMax.length()){Segment = "0" + Segment;}
				Result+=Segment;
				++it;
			}

			return Result;
		}

		/** Divides the contents of two numeric vectors. Vectors must be of same length */
		Coordinate DivideCoordinate(Coordinate Dividend, \
				Coordinate Divisor){

			Coordinate Quotient;
			if(Dividend.size() != Divisor.size()){
				cout<<"GRID:\t\t ERROR! Dimension vector improper length.";
				return Quotient;//can't do the math. return an empty vector
			}
			for(int i = 0; i < Divisor.size(); i++)
				if(Divisor[i] ==0 || Divisor.empty())
				{
					cout<<"ERROR: Cannot devide by zero.\n";
					return Quotient;//can't do the math. return an empty vector
				}

			Coordinate::iterator itDividend = Dividend.begin();
			Coordinate::iterator itDivisor = Divisor.begin();
			while(itDividend != Dividend.end()){
				//Divide each pair of values from the given vectors and place 
				//in our solution vector.
				Quotient.push_back( floor((*itDividend) / (*itDivisor)) );

				++itDividend;
				++itDivisor;
			}
			return Quotient;
		}

		//SetPtrObj GetNeighbours(PtrObj O, Coordinate L){
		//	string Key = GetKey(L);
		//	cout<<Key<<"\n";
		//	map<string, PtrObj>::iterator it = Cells.find(Key);
		//	PtrObj Cell = ((*it).second);

		//	SetPtrObj Result;
		//	vector<boost::shared_ptr<Object> >::iterator i=Cell->BoundObjs.begin();
		//	while(i!=Cell->BoundObjs.end()){
		//		if(*i!=O){
		//			//we dont include O in the list of objects near O.
		//			Result.insert(*i);
		//			++i;
		//		}
		//	}
		//	
		//	return Result;
		//}


		///Helper function for RFoil()
		vector<Coordinate> FindCellOverlap(PtrObj O){
	
			//Our RFoil function will use LCopy and ObjectSizeCopy
			//to create all combinations of cells the object touches.
			//these combinations are built and temporarily stored in Input.
			boost::shared_ptr<Coordinate> Input(new Coordinate);
	
			return RFoil(O, O->Location, O->Dimensions, this->CellSize, Input);
		}

		//Returns the keys to all cells object O intersects with
		//since this involves looping through all N dimensions and N is unknown,
		//recursion is used rather than iteration.
		vector<Coordinate> RFoil(PtrObj O, Coordinate L, \
				Size S, Size CS, boost::shared_ptr<Coordinate> Instance){

			vector<Coordinate> Result;
			vector<Coordinate> Returned;

			if(CS.size() == 0 ){//base case
				Result.push_back(*Instance);
				return Result;
			}else{//recursive case

				//Here we break off a level of the problem for this recursive
				//instance to handle.
				Scalar FirstLoc = *(L.begin());
				Scalar FirstSize = *(S.begin());
				Scalar FirstCellSize = *(CS.begin());

				//reduction of the problem
				L.erase(L.begin());
				S.erase(S.begin());
				CS.erase(CS.begin());

				//loop through this level of the problem while iterating 
				//through the reduced problem
				int Start = floor(FirstLoc / FirstCellSize);
				int End = floor((FirstLoc+FirstSize) / FirstCellSize);
				for(int i = Start; i <= End;i++){
					//we run the simplified problem for each cell the object spans
					//in this particular dimension.
					Instance->push_back(i);
					Returned = RFoil(O,L,S,CS,Instance);
					Result.insert(Result.end(),Returned.begin(),Returned.end());
					Instance->pop_back();
				}
			}
			return Result;
		}

		/**	Adds O to location L iff it was not already there*/
		void Add(PtrObj O, Coordinate L){
			bool Duplicate = true;//we begin by assuming this object is already here

			//check if the cell exists. if not, make it.
			PtrObj Cell;
			string Key = GetKey(L);
			map<string, PtrObj>::iterator it = Cells.find(Key);

			if(it == Cells.end()){//cell does not exist
				Cell = PtrObj(new AABB);

				//Give the cell volume and place it
				
				Coordinate::iterator Ci = L.begin();
				Size::iterator Si = CellSize.begin();
				while(Si != CellSize.end()){
					Cell->Dimensions.push_back(*Si);
					Cell->Location.push_back((*Ci)*(*Si));
					++Si;
					++Ci;
				}

				pair<string, PtrObj > NewCell (Key,Cell);
				Cells.insert(NewCell);
				Duplicate = false; //object cant be in a cell that was just created
			}else{
				Cell = (*it).second;
			}

			if(Duplicate){//if we havent already ruled out a duplicate
				//check the cell for the object
				bool match = false;

				ListPtrObj::iterator it;
				ListPtrObj CellContents = Cell->BoundObjs;
				it = CellContents.begin();

				while(it != CellContents.end()){
					if((*it).get() == O.get()){
						match = true;
					}else{}
					++it;
				}
				Duplicate = match;
			}

			/*add object to cell if it isnt already here*/
			if(!Duplicate){
				Cell->Add((Object::PtrObj)O);

				if(this->debug){
					Coordinate::iterator itLoc = L.begin();
					cout<<"GRID:\t\t inserting into cell(";
					while(itLoc != L.end()){
						cout<<(*itLoc);
						++itLoc;
						if(itLoc != L.end())
							cout<<",";
					}
					cout<<") ("<<Cell->BoundObjs.size()<<" objects currently in cell)\n";
				}
			}else{
				if(this->debug){
					cout<<"GRID:\t\t Duplicate object inserted. Ignoring.\n";
				}
			}
		}

		/**	Removes O from location L if present. Removes any cells that become empty due to this removal. */
		void Remove(PtrObj O, Coordinate L){
			//check if the cell exists. if not the object is not here and we are done
			PtrObj Cell = this->CellAt(L);
			if(!Cell.get())
				return;

			//remove the object if it is in the cell
			ListPtrObj::iterator it;
			it = Cell->BoundObjs.begin();
			while(it != Cell->BoundObjs.end()){
				if((*it).get() == O.get()){
					Cell->BoundObjs.erase(it);
					break;
				}else{
					++it;
				}
			}

			if(this->debug){
				Coordinate::iterator itLoc = L.begin();
				cout<<"GRID:\t\t deleting from cell (";
				while(itLoc != L.end()){
					cout<<(*itLoc);
					++itLoc;
					if(itLoc != L.end())
						cout<<",";
				}
				cout<<") ("<<Cell->BoundObjs.size()<<" objects currently in cell)\n";
			}

			//if the cell is now empty we remove the entire cell to save space
			if(Cell->BoundObjs.size() == 0){
				Cells.erase(this->GetKey(L));
				cout<<"GRID:\t\t Empty cell! Removing.\n";
			}
		}

		/** Returns the contents of the cell containing a specific point*/
		//TODO:Return ListPtrObject, not PtrListPtrObj
		PtrObj CellAt(Coordinate Location){
			map<string, PtrObj>::iterator it;
			it = Cells.find(GetKey(Location));
			
			if(it == Cells.end()) {
				PtrObj Empty;
				//TODO return object copy, not ptr.
				//return Empty;//cell does not exist.
			}else{
				return (*it).second;
			}
		}

	public:
		
		///Constructor permenantly sets the size of each cell
		singlegrid(Size CellDimensions){
			CellSize = CellDimensions;
		}
		
		/**	Gets all objects that are in any cell O overlaps*/
		//SetPtrObj GetNeighbours(PtrObj O){
		//	vector<Coordinate> Cells = FindCellOverlap(O);
		//	vector<Coordinate>::iterator i = Cells.begin();

		//	SetPtrObj temp;
		//	SetPtrObj Result;

		//	while(i != Cells.end()){
		//		temp =  GetNeighbours(O,*i);
		//		Result.insert(temp.begin(),temp.end());
		//		++i;
		//		cout<<"Here again\n";
		//	}

		//	return Result;
		//}

		/**
			Places object into grid
		*/
		void Add(PtrObj O){
			vector<Coordinate> OCells = FindCellOverlap(O);
			vector<Coordinate>::iterator i = OCells.begin();
			while(i!=OCells.end()){
				Add(O,(*i));
				++i;
			}
		}

		/**	Adds all objects in List to the data structure. 
				Will not put a duplicate object into the grid. */
		void Add(SetPtrObj List){
			SetPtrObj::iterator it;
			it = List.begin();
			while(it!=List.end()){
				Add(*it);
				++it;
			}
		}
		
		/**
			Removes object from grid
		*/
		void Remove(PtrObj O){
			vector<Coordinate> OCells = FindCellOverlap(O);
			vector<Coordinate>::iterator i = OCells.begin();
			while(i!=OCells.end()){
				Remove(O,(*i));
				++i;
			}
		}
		/** Removes all objects in List from the data structure as well as any empty cells. */
		void Remove(ListPtrObj List){
			ListPtrObj::iterator it;
			it = List.begin();
			while(it!=List.end()){
				Remove(*it);
				++it;
			}
		}		 

		//TODO: use a set to trim duplicates from list.
		/**	Retrieves all objects contained within the data structure */
		SetPtrObj Export()
		{
			SetPtrObj Result;
			//iterate through all cells in a map
			map<string, PtrObj>::iterator itMap;
			//iterate through all objects in a cell
			ListPtrObj::iterator itCell;

			//get proper length zero vector so we can reference the start of the grid
			//TODO: What if we have negative coordinates?
			Coordinate Zero;
			for(unsigned int i = 0; i<CellSize.size();i++){
				Zero.push_back(0);
			}

			//place iterator at the first element in Cells.
			itMap = Cells.begin();

			while(itMap!=Cells.end()){//for each cell...
				itCell = (*itMap).second->BoundObjs.begin();
				//...go through all objects in cell
				while(itCell!=(*itMap).second->BoundObjs.end()){
					Result.insert(*itCell);//and add these objects to Result
					++itCell;
				}
				++itMap;
			}
			return Result;
		}

		/** Erases all objects from the data structure
				The CellSize data is maintained. */
		void Clear(){
			Cells.erase(Cells.begin(), Cells.end());
		}

		/**	Returns the total number of cells in the data structure */
		Scalar CellCount(){
			return Cells.size();
		}

		/**	Returns the list of cell dimensions */
		Size GetCellSize(){ return CellSize; }

		/** Returns the magnitude of the i'th dimension in the cell. 
			i must be a number between 0 and CellsSize.size()-1 */
		Scalar GetCellSize(int i){ 
			if(i >= 0 && i <= CellSize.size()-1)
				return CellSize.at(i); 
			return -1;//Assuming cells cant be a negative length, this should work.
		} 

		SetPtrObj GetNeighbours(PtrObj O){
			SetPtrObj Results;

			//Get all the cells O contacts
			vector<Coordinate> CellLocations = FindCellOverlap(O);
			
			PtrObj SingleCell = (Cells.find(GetKey(CellLocations.back()))->second);
			ListPtrObj::iterator j = SingleCell->BoundObjs.begin();
			while(CellLocations.size() > 0){
				while(j != SingleCell->BoundObjs.end()){
					Results.insert((*j));
					++j;
				}
				CellLocations.pop_back();
				if(CellLocations.size()>0){
					SingleCell = (Cells.find(GetKey(CellLocations.back()))->second);
					j = SingleCell->BoundObjs.begin();
				}
			}

			//O is not a Neighbour of itself.
			Results.erase(O);
			return Results;

		}

		class const_iterator : public iterator<forward_iterator_tag,PtrObj>{
		
			//Private constructor means only hgrid can create this iterator
			friend class singlegrid;

			private:
				map<string, PtrObj>::iterator Iter;

				//const_iterator(PtrObj NewCurrent): Current(NewCurrent){}
				const_iterator(map<string, PtrObj>::iterator newIter){
					Iter = newIter;
				}

			public:
				PtrObj operator*(){
					return (*Iter).second;
				}
				const const_iterator & operator++(){
					++Iter;
					return *this;
				}
				bool operator !=(const const_iterator& external) const{
					return this->Iter != external.Iter;
				}
		};


		singlegrid::const_iterator begin(){return Cells.begin();}
		singlegrid::const_iterator end(){return Cells.end();}
};

#endif
