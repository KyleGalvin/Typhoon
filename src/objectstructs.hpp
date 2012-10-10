#ifndef __OBJECTSTRUCTS_HPP__
#define __OBJECTSTRUCTS_HPP__

#include "SDL/SDL_opengl.h"
#include "vector"
#include "set"
#include "geotypes.hpp"
#include "math.h"
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

class Force{
	public:
		std::shared_ptr< Coordinate > direction;
		float magnitude;		
};

class Object{
	public:

	shared_ptr<Coordinate> Up;
	shared_ptr<Coordinate> Target;
	shared_ptr<Coordinate> Right;

	float xVel;
	float yVel;
	float zVel;

	float xAcc;
	float yAcc;
	float zAcc;

	float mass;
	float friction;
	float maxSpeed;

	void calculateAcceleration(Coordinate NetForce){
		xAcc = (NetForce[0]/mass)-(friction*xVel);
		yAcc = (NetForce[1]/mass)-(friction*yVel);
		zAcc = (NetForce[2]/mass)-(friction*zVel);
	}
	void Normalize(shared_ptr<Coordinate> vec){
		//normalize!
		float len = sqrt(((*vec)[0] * (*vec)[0])+((*vec)[1] * (*vec)[1])+((*vec)[2] * (*vec)[2]));
		(*vec)[0]/=len;
		(*vec)[1]/=len;
		(*vec)[2]/=len;
	}
	void accUp(float y){
		yAcc +=y;
	}

	void accRight(float x){
		xAcc +=x;
	}

	void accForward(float z){
		zAcc +=z;
	}

	void velUp(float y){
		yVel += y;
	}
	void velRight(float x){
		xVel += x;
	}
	void velForward(float z){
		zVel += z;
	}
	shared_ptr<Force>  addForce(shared_ptr<Coordinate> unitVector, float magnitude){

		shared_ptr<Force> f (new Force());
		f->magnitude = magnitude;	
		f->direction = unitVector;	

		Forces.push_back(f);
		return f;
	}
	void rotateX(float amount)
	{
		//mod 360 for sane numbers
		int IntAmount = (int) amount;
		int AdjustedIntAmount = IntAmount % 360;
		//save the decimal/fraction
		amount -= IntAmount;
		//add the simplified whole with the fraction
		amount +=AdjustedIntAmount;

		Coordinate tempTarget = *Target;
		Coordinate tempUp = *Up;

		amount /=57.2957795f; // convert degrees to radians
		
		(*Target)[0] = (cos(amount) * tempTarget[0]) - (sin(amount) * tempUp[0]);
		(*Target)[1] = (cos(amount) * tempTarget[1]) - (sin(amount) * tempUp[1]);
		(*Target)[2] = (cos(amount) * tempTarget[2]) - (sin(amount) * tempUp[2]);

		(*Up)[0] = (cos(amount) * tempUp[0]) + (sin(amount) * tempTarget[0]);
		(*Up)[1] = (cos(amount) * tempUp[1]) + (sin(amount) * tempTarget[1]);
		(*Up)[2] = (cos(amount) * tempUp[2]) + (sin(amount) * tempTarget[2]);

		Normalize(Target);	
		Normalize(Up);
	}

	void rotateY(float amount)
	{
		//mod 360 for sane numbers
		int IntAmount = (int) amount;
		int AdjustedIntAmount = IntAmount % 360;
		//save the decimal/fraction
		amount -= IntAmount;
		//add the simplified whole with the fraction
		amount +=AdjustedIntAmount;
		
		Coordinate tempTarget = *Target;
		Coordinate tempRight = *Right;

		amount /=57.2957795f; // convert degrees to radians

		(*Target)[0] = (cos(amount) * tempTarget[0]) + (sin(amount) * tempRight[0]);
		(*Target)[1] = (cos(amount) * tempTarget[1]) + (sin(amount) * tempRight[1]);
		(*Target)[2] = (cos(amount) * tempTarget[2]) + (sin(amount) * tempRight[2]);


		(*Right)[0] = (cos(amount) * tempRight[0]) - (sin(amount) * tempTarget[0]);
		(*Right)[1] = (cos(amount) * tempRight[1]) - (sin(amount) * tempTarget[1]);
		(*Right)[2] = (cos(amount) * tempRight[2]) - (sin(amount) * tempTarget[2]);

		Normalize(Right);
		Normalize(Target);	
	}

	void rotateZ(float amount)
	{
		//mod 360 for sane numbers
		int IntAmount = (int) amount;
		int AdjustedIntAmount = IntAmount % 360;
		//save the decimal/fraction
		amount -= IntAmount;
		//add the simplified whole with the fraction
		amount +=AdjustedIntAmount;
		
		Coordinate tempRight = *Right;
		Coordinate tempUp = *Up;
		
		amount /=57.2957795f; // convert degrees to radians

		(*Up)[0] = (cos(amount) * tempUp[0]) - (sin(amount) * tempRight[0]);
		(*Up)[1] = (cos(amount) * tempUp[1]) - (sin(amount) * tempRight[1]);
		(*Up)[2] = (cos(amount) * tempUp[2]) - (sin(amount) * tempRight[2]);

		(*Right)[0] = (cos(amount) * tempRight[0]) + (sin(amount) * tempUp[0]);
		(*Right)[1] = (cos(amount) * tempRight[1]) + (sin(amount) * tempUp[1]);
		(*Right)[2] = (cos(amount) * tempRight[2]) + (sin(amount) * tempUp[2]);

		Normalize(Up);
		Normalize(Right);	
	}

	void translateUp(float amount){
		Location[0]+= (*Up)[0]*amount;
		Location[1]+= (*Up)[1]*amount;
		Location[2]+= (*Up)[2]*amount;
	}
	void translateForward(float amount){
		Location[0]+= (*Target)[0]*amount;
		Location[1]+= (*Target)[1]*amount;
		Location[2]+= (*Target)[2]*amount;
	}
	void translateRight(float amount){
		Location[0]+= (*Right)[0]*amount;
		Location[1]+= (*Right)[1]*amount;
		Location[2]+= (*Right)[2]*amount;
	}
	
	void translateX(float amount){
		Location[0]+=amount;
	}

	void translateY(float amount){
		Location[1]+=amount;
	}

	void translateZ(float amount){
		Location[2]+=amount;
	}
		typedef Object Obj;
		typedef std::shared_ptr<Obj> PtrObj;
		typedef vector<PtrObj> ListPtrObj;

		typedef vector<Line> LineList;
		typedef std::shared_ptr<LineList> PtrLineList;

		//the object bound within the limits of this object.
		//if this is the main object and no lower level exists, 
		//then this value is null.
		ListPtrObj BoundObjs;

		vector< shared_ptr<Force> > Forces;

		Coordinate Location;
		Size Dimensions;
		Size Rotation;

		bool debug;

		int GetComplexity(){return Complexity;}

		void Add(PtrObj O){BoundObjs.push_back(O);}


		//when we call this, we must be sure the object is 3d
		PtrLineList Draw3DQuads(){
			Coordinate::iterator Di = Dimensions.begin();
			Coordinate::iterator Li = Location.begin();

			glBegin(GL_QUADS);

			//front
			glTexCoord2f(0.0,0.0);
			glVertex3f(Li[0]+Di[0],Li[1],Li[2]);
			glTexCoord2f(0.0,1.0);
			glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]);
			glTexCoord2f(1.0,1.0);
			glVertex3f(Li[0],Li[1]+Di[1],Li[2]);
			glTexCoord2f(1.0,0.0);
			glVertex3f(Li[0],Li[1],Li[2]);

			//top
			glTexCoord2f(0.0,0.0);
			glVertex3f(Li[0],Li[1],Li[2]);
			glTexCoord2f(0.0,1.0);
			glVertex3f(Li[0]+Di[0],Li[1],Li[2]);
			glTexCoord2f(1.0,1.0);
			glVertex3f(Li[0]+Di[0],Li[1],Li[2]+Di[2]);
			glTexCoord2f(1.0,0.0);
			glVertex3f(Li[0],Li[1],Li[2]+Di[2]);

			//bottom
			glTexCoord2f(0.0,0.0);
			glVertex3f(Li[0],Li[1]+Di[1],Li[2]);
			glTexCoord2f(0.0,1.0);
			glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]);
			glTexCoord2f(1.0,1.0);
			glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]+Di[2]);
			glTexCoord2f(1.0,0.0);
			glVertex3f(Li[0],Li[1]+Di[1],Li[2]+Di[2]);
		
			//back
			glTexCoord2f(0.0,0.0);
			glVertex3f(Li[0],Li[1],Li[2]+Di[2]);
			glTexCoord2f(0.0,1.0);
			glVertex3f(Li[0]+Di[0],Li[1],Li[2]+Di[2]);
			glTexCoord2f(1.0,1.0);
			glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]+Di[2]);
			glTexCoord2f(1.0,0.0);
			glVertex3f(Li[0],Li[1]+Di[1],Li[2]+Di[2]);

			//left
			glTexCoord2f(0.0,0.0);
			glVertex3f(Li[0],Li[1],Li[2]+Di[2]);
			glTexCoord2f(0.0,1.0);
			glVertex3f(Li[0],Li[1]+Di[1],Li[2]+Di[2]);
			glTexCoord2f(1.0,1.0);
			glVertex3f(Li[0],Li[1]+Di[1],Li[2]);
			glTexCoord2f(1.0,0.0);
			glVertex3f(Li[0],Li[1],Li[2]);
			
			//right
			glTexCoord2f(0.0,0.0);
			glVertex3f(Li[0]+Di[0],Li[1],Li[2]);
			glTexCoord2f(0.0,1.0);
			glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]);
			glTexCoord2f(1.0,1.0);
			glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]+Di[2]);
			glTexCoord2f(1.0,0.0);
			glVertex3f(Li[0]+Di[0],Li[1],Li[2]+Di[2]);
		}

		//when we call this, we must be sure the object is 2d
		PtrLineList Draw2DQuads(){
			Coordinate::iterator Di = Dimensions.begin();
			Coordinate::iterator Li = Location.begin();

			glVertex3f(Li[0],Li[1],Li[2]);


		}

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
								while(Ci != PreviousV.end()){
									++Ci;
								}
								Ci = CurrentV.begin();
								while(Ci != CurrentV.end()){
									++Ci;
								}
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
