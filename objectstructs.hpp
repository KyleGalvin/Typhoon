#ifndef __OBJECTSTRUCTS_HPP__
#define __OBJECTSTRUCTS_HPP__

#include "vector"
#include "set"
#include "geotypes.hpp"
#include <iostream>
#include <memory>
using namespace std;

//Here we define algorithm complexity levels for different types of bounding
//hulls. When computing a collision between two arbitrary bounding shapes,
//the algorithm we use will correspond with the more complex of the two shapes.
//More complex shapes use more complex algorithms.
#define COMP_AABB 1 //Axis Aligned Bounding Boxes
#define COMP_OOB 2 //Oriented Object Box
#define COMP_CH 3 //Convex Hull
#define COMP_AH 4 //Arbitrary Hull

class Polygon{
	public:
		vector<Coordinate> Poly;//a line loop of points forms the polygon
};

typedef pair<Coordinate,Coordinate> Line;

class Object{
	public:

		typedef Object Obj;
		typedef std::shared_ptr<Obj> PtrObj;
		typedef vector<PtrObj> ListPtrObj;

		typedef vector<Line> LineList;
		typedef std::shared_ptr<LineList> PtrLineList;

		//the object bound within the limits of this object.
		//if this is the main object and no lower level exists, 
		//then this value is null.
		ListPtrObj BoundObjs;

		Coordinate Location;
		Size Dimensions;
		Size Rotation;

		bool debug;

		int GetComplexity(){return Complexity;}

		void Add(PtrObj O){BoundObjs.push_back(O);}

		//This will return a list of polygons that can be used to explicitly 
		//outline the bounding structure. This is being created for collision
		//debugging, and may not be appropriate for rendering complex arbitrary 
		//hulls due to many redundant shared verticies.
		PtrLineList GetShape(){
			//Create a wireframe for our AABB

			//Store the wires we've found
			PtrLineList result(new LineList);

			//We are finished when we run out of verticies to consider
			set<Coordinate> WorkingVerticies;
			set<Coordinate> WorkingVerticiesSwapBuffer;

			//add our first vertex as a seed

			WorkingVerticies.insert(Location);

			Coordinate CurrentV;
			Coordinate::iterator CVi;
			Coordinate PreviousV;

			//to iterate over our location and dimensions
			Coordinate::iterator Li;
			Coordinate::iterator Di;

			Line L;

			//When both vertex buffers are empty, we exit
			while(WorkingVerticies.size() > 0){

				//When one vertex buffer is empty, we swap the new one in its place
				while(WorkingVerticies.size() > 0){
					//(re)set our iterators
					Di = Dimensions.begin();
					Li = Location.begin();

					//begin processing the first vertex.
					CurrentV = PreviousV = *WorkingVerticies.begin();
					CVi = CurrentV.begin();

					//Go through our location dimension by dimension.
					while(CVi!=CurrentV.end()){
						//if this dimension has not been incremented to its extreme
						if(*CVi == *Li){
							//increment it to it's extreme
							*CVi = (*Li) + (*Di);
							//verticies in the swap buffer will be processed
							//in the next iterative pass.
							WorkingVerticiesSwapBuffer.insert(CurrentV);
							//Add our new line segment
							L.first = PreviousV;
							L.second = CurrentV;
							result->push_back(L);

							if(debug){
								Coordinate::iterator Ci = PreviousV.begin();
								cout<<"Adding Line:(";
								while(Ci != PreviousV.end()){
									cout<<(*Ci);
									++Ci;
									if(Ci != PreviousV.end())
										cout<<",";
								}
								cout<<") - (";
								Ci = CurrentV.begin();
								while(Ci != CurrentV.end()){
									cout<<(*Ci);
									++Ci;
									if(Ci != CurrentV.end())
										cout<<",";
								}
								cout<<")\n";
							}

							//Undo our increment and move on
							*CVi = *Li;
						}
						++CVi;
						++Li;
						++Di;
					}

					//We are done processing this vertex. Remove it.
					WorkingVerticies.erase(WorkingVerticies.begin());
				}

				//Dump swap buffer into WorkingVerticies. if swap.size()>0, we iterate.
				WorkingVerticies = WorkingVerticiesSwapBuffer;
				WorkingVerticiesSwapBuffer.clear();
			}
			return result;
		}

	protected:

		int Complexity;
};

//TODO: Sizes cannot be negative! check on push_back?
class AABB: public Object{
	public:
		AABB(){Complexity = COMP_AABB;}

		//Creates a minimum-area AABB around Object O
		//bool BindObject(Object* O){
		//	
		//	vector<Coordinate>::iterator ShapePoint = O->GetShape().begin();
		//	//vector<Scalar>::iterator 
		//	
		//}
};

//class AH: public Object{
//	vector<Line*> Shape;//a list of lines binds the area?
//	AH(){Complexity = COMP_AH;}
//	vector<Line>* GetShape(){return &Shape;}
//};

#endif
