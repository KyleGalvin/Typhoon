#ifndef __GRID_HPP__
#define __GRID_HPP__

#include <vector>
#include <memory>
#include "geotypes.hpp"
#include "collision.hpp"
#include <set>
using namespace std;

typedef Object Obj;
typedef std::shared_ptr<Obj> PtrObj;
typedef vector<PtrObj> ListPtrObj;
typedef std::shared_ptr<ListPtrObj> PtrListPtrObj;
typedef set<PtrObj> SetPtrObj;
typedef std::shared_ptr<SetPtrObj> PtrSetPtrObj;
typedef vector<Scalar> ListScalar;
typedef std::shared_ptr<ListScalar> PtrListScalar;

/**A map containing lists of objects used for basic spatial partitioning.
	Spatial partitioning is a requirement in complex collision detection systems.

	This abstract class holds the basic members and member variables used in the
	various base-class grids derived from this.

	we use a map to draw a relation between cells and coordinates and we use a
	vector to store objects in each cell in a bucket fashion.*/
class grid{

	public:


		bool debug;

		virtual void Add(PtrObj) = 0;
		virtual void Add(SetPtrObj List) = 0;
		virtual void Remove(PtrObj) = 0;
		virtual void Remove(ListPtrObj List) = 0;
		virtual PtrSetPtrObj GetNeighbours(PtrObj) = 0;
		virtual SetPtrObj Export() = 0;
		virtual void Clear() = 0;
		virtual Scalar CellCount() = 0;
};

#endif
