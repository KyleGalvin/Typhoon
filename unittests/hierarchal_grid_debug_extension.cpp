#include "../grid.hpp"
#include <iostream>

class HGDE : public hgrid{
	public: HGDE(){
		cout<<"hgde constructor\n";
	}

	public: virtual void Add(SetPtrObj List){
		SetPtrObj::iterator it;
		it = List.begin();
		while(it!=List.end()){
			Add(*it);
			++it;
		}
	}

	public: virtual void Add(PtrObj O){
		cout<<"hgde add\n";
		hgrid::Add(O);

		int cellsum=0;
		int fragmentsum=0;
		for(int i=0; i<grids.size();i++){
			cout<<"CellCount for grid "<<i<<"="<<grids[i]->CellCount()<<"\n";
			singlegrid::const_iterator it = grids[i]->begin();
			while(it!=grids[i]->end()){
				
				cout<<"Cell contains"<<(*it)->BoundObjs.size()<<"\n";
				fragmentsum+=(*it)->BoundObjs.size();
				++it;
				++cellsum;
			}
		}

		cout<<"Structure contains "<<cellsum<<" cells and "<<fragmentsum<<" object fragments\n";
	}
};
