#include <list>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <typeinfo>
#include <iterator>
#include "grid.hpp"
#include "singlegrid.hpp"
using namespace std;


class hgrid: public grid{

	typedef vector<PtrObj > ListPtrObj;
	typedef auto_ptr<ListPtrObj> PtrListPtrObj;
	typedef vector<Scalar> NumBucket;
	typedef auto_ptr<NumBucket> PtrNumBucket;

	protected:
		vector<std::shared_ptr<singlegrid> > grids;

		///Returns the largest side of object O
		Scalar LargestDimension(PtrObj O){
			Coordinate::iterator it = O->Dimensions.begin();
			Scalar result = *it;
			//go through all dimensions of our coordinates and makes a unique string from them
			while(it!=O->Dimensions.end()){
				if((*it) > result) result = (*it);
				++it;
			}
			return result;
		}

		bool IsEmpty(){
			if(grids.size() == 0) return true;
			return false;
		}

		///Places the given grid where it belongs in the grids vector.
		bool PlaceGrid(std::shared_ptr<singlegrid> grid){
			//since our grids are all square, the first size speaks for all.
			Scalar NewSize = grid->GetCellSize(0);

			vector<std::shared_ptr<singlegrid> >::iterator i = grids.begin();
			bool placed = false;
			Scalar CompSize;

			//iterate through the list of grids until we find an appropriate place
			//to insert the new grid	
			while(i != grids.end() && placed == false){
				CompSize = (*i)->GetCellSize(0);
				if(NewSize > CompSize){
					grids.insert(i, grid);
					placed = true;
					break;
				}
				++i;
			}
			//if placed is still false it is because this grid is smaller than all
			//of our existing grids. We can simply place it at the end of the list.
			if(!placed){
				grids.push_back(grid);
			}

			if(debug){
				cout<<"HGRID:\t\t New grid added. Grid stack currently:\n";
				i=grids.begin();
				int n = 0;
				while(i != grids.end()){
					cout<<"HGRID:\t\t		--> Grid("<<n<<") size "<<(*i)->GetCellSize(0)<<"\n";
						cout<<"HGRID:\t\t Grid Cell Size: "<<(*i)->GetCellSize(0)<<"gcc:"<<(*i)->CellCount()<<"\n";
					++n;
					++i;
				}
			}

			return true;
		}

		///Creates a grid that both fits O within it and fits in our grids list.
		std::shared_ptr<singlegrid> MakeGrid(PtrObj O){
			Scalar NewGridSize;
			Scalar Largest = LargestDimension(O);

			if(grids.size() == 0){
				//starting a fresh hgrid is its own case
				NewGridSize = Largest;
			}else{
				if(grids.at(0)->GetCellSize(0) < Largest){
					//new grid bigger than the biggest.
					Scalar base = grids.at(0)->GetCellSize(0);

					while(Largest > base){
						base *=2;
					}
					NewGridSize = base;		

				}else{
					//new grid smaller than the biggest.
					Scalar base = grids.at(0)->GetCellSize(0);
					while(Largest < base/2){
						base /=2;
					}
					NewGridSize = base;

				}

			}

			Size S;
			for(unsigned int i=0; i<O->Dimensions.size(); i++){
				S.push_back(NewGridSize);
			}
			std::shared_ptr<singlegrid> NewGrid(new singlegrid(S));

			if(debug){
				cout<<"HGRID:\t\t Creating new grid. Size: "<<NewGridSize<<"\n";
			}


			NewGrid->debug = debug;
			return NewGrid;
		}

	public:
		bool debug;

		hgrid(){};

		/**Scans through our grids to find an appropriate grid for the object.
			If no proper grid is found one is made. O is then added to the grid.*/
		void Add(PtrObj O){

			Scalar Large = LargestDimension(O);

			if(debug){
				cout<<"HGRID:\t\t Add new object. size: "<<Large<<"\n";
			}

			if(grids.size()>0){

				//iterate through the list of grids searching for a place
				//to insert the new object
				vector<std::shared_ptr<singlegrid> >::iterator i = grids.begin();
				while(Large >= (*i)->GetCellSize(0) || Large < (*i)->GetCellSize(0) ){
					++i;
					if(i==grids.end()){
						i--;//grids.end() is a past-the-end element. Dont overrun!
						break;
					}
				}

				cout<<"Large:"<<Large<<" CellSize:"<<(*i)->GetCellSize(0)<<"\n";
				//if we have successfully found a good grid, add the object and return.
				if(Large > (*i)->GetCellSize(0)/2 && Large <= (*i)->GetCellSize(0) && i !=grids.end()){
					cout<<"HGRID:\t\t Proper grid found. Placing object.\n";
					(*i)->Add(O);
					cout<<"HGRID:\t\t Grid Cell Size: "<<(*i)->GetCellSize(0)<<"gcc:"<<(*i)->CellCount()<<"\n";

					return;
				}
			}

			//if we havent returned yet, no appropriate grid exists. Make one.
			if(debug){
				cout<<"HGRID:\t\t No proper grid found.\n";
			}

			std::shared_ptr<singlegrid> NewGrid = std::shared_ptr<singlegrid>(MakeGrid(O));
			PlaceGrid(NewGrid);
			NewGrid->Add(O);
		}

		///Calls Add(AABB) repeatedly for every object in List vector
		void Add(SetPtrObj List){
			SetPtrObj::iterator it;
			it = List.begin();
			while(it!=List.end()){
				Add(*it);
				++it;
			}
		}

		/**Searches for a grid that O may fit in. If one is found, delete O from
			that grid. If not, the object can't already be in our structure.*/
		void Remove(PtrObj O){
			//find the appropriate grid
			Scalar Large = LargestDimension(O);

			vector<std::shared_ptr<singlegrid> >::iterator i = grids.begin();
			while(Large < (*i)->GetCellSize(0)/2 && i != grids.end()){
				++i;
			}

			//if no apropriate grid found, the object is not in the structure.
			if(i==grids.end()){
				return;
			}else{
				//if the grid is found, remove grid from the structure.
				(*i)->Remove(O);

				if((*i)->CellCount() == 0){
					grids.erase(i);
					if(debug){
						cout<<"HGRID:\t\t Empty grid! Removing.\n";
					}
				}
			}
		}

		///calls Remove(AABB repeatedly for every object in the List vector
		void Remove(ListPtrObj List){
			ListPtrObj::iterator it;
			it = List.begin();
			while(it!=List.end()){
				Remove(*it);
				++it;
			}
		}		

		PtrListPtrObj CellAt(Coordinate Location){}

		///Returns a list of all lists exported from the sub-grids.
		SetPtrObj Export(){

			SetPtrObj Result;
			SetPtrObj Temp;

			vector<std::shared_ptr<singlegrid> >::iterator i = grids.begin();
			while(i!=grids.end()){
				//the hgrid export is the sum of all it's grid exports.
				Temp = (*i)->Export();
				Result.insert(Temp.begin(),Temp.end());
				++i;
			}

			return Result;
		}

		///Clears and erases all sub-grids.
		void Clear(){
			bool temp = true;
			bool result = true;
			//clear all subgrids
			vector<std::shared_ptr<singlegrid> >::iterator i = grids.begin();
			while(i<grids.end()){
				(*i)->Clear();
				++i;
			}

			//erase all subgrids
			grids.erase(grids.begin(),grids.end());
		}

		PtrSetPtrObj GetNeighbours(PtrObj O){

			PtrSetPtrObj Result;
			PtrSetPtrObj PartialResult;
			SetPtrObj::iterator PRit;
			
			for(int i=0;i<grids.size();i++){
				PartialResult = grids[i]->GetNeighbours(O);

				PRit = PartialResult->begin();
				while(PRit != PartialResult->end()){
					Result->insert((*PRit));
					++PRit;
				}
			}

			return Result;
		}

		///The sum of all cells in all grids.
		Scalar CellCount(){
			Scalar result = 0;
			vector<std::shared_ptr<singlegrid> >::iterator i = grids.begin();
			while(i<grids.end()){
				result+=(*i)->CellCount();
				++i;
			}

			return result;
		}

		int GridCount(){
			return grids.size();
		}

		class const_iterator : public iterator<forward_iterator_tag,PtrObj>{
			
			//Private constructor means only hgrid can create this iterator
			friend class hgrid;
				hgrid* myContainer;	
				vector<std::shared_ptr<singlegrid> >::iterator Outer;
				singlegrid::const_iterator Inner;
			private:

				const_iterator(vector<std::shared_ptr<singlegrid> >::iterator newOuter, singlegrid::const_iterator newInner, hgrid* newContainer): Outer(newOuter),Inner(newInner){myContainer=newContainer;}
				//const_iterator(const hgrid::const_iterator& i): Outer(i.Outer), Inner(i.Inner){}
			public:
				PtrObj operator*(){
					return *Inner;
				}

				const const_iterator & operator++(){
					++Inner;
					if(Inner == (**Outer).end()){
						++Outer;
						if(Outer != myContainer->grids.end()){
							Inner = (**Outer).begin();
						}
					}
				
					return *this;
				};

				const const_iterator & operator--(){
					--Inner;
					if(Inner ==(**Outer).begin()){
						--Outer;
						Inner=(**Outer).end();
					}

					return *this;
				}

				bool operator !=(const const_iterator& external) const{
					return this->Inner != external.Inner;
				}
				bool operator ==(const const_iterator& external) const{
					return this->Inner == external.Inner;
				}
		};
		
		const_iterator begin(){
			const_iterator it(grids.begin(),(*grids.begin())->begin(),this);
			return it;
		}

		const_iterator end(){
			const_iterator it(grids.end(),(*(--grids.end()))->end(),this);
			return it;
		}
		
};

