#ifndef __COLLISION_HPP__
#define __COLLISION_HPP__

#include "objectstructs.hpp"

class CollisionEngine{
	public:
		bool TestAABB(AABB* O1, AABB* O2){

			//L is the beginning of our object square
			//L+D is the end.
			vector<Scalar>::iterator Li1 = O1->Location.begin();
			vector<Scalar>::iterator Li2 = O2->Location.begin();
			vector<Scalar>::iterator Di1 = O1->Dimensions.begin();
			vector<Scalar>::iterator Di2 = O2->Dimensions.begin();

			//we assume a collision occurs until we can find an axis
			//which divides the two objects.
			bool collision = true;

			while(Li1 != O1->Location.end() && Li2 != O2->Location.end()){
				if((*Li1)>((*Li2)+(*Di2)) || ((*Li1)+(*Di1))<(*Li2)){
					//if the beginning of obj1 is past the end of obj2
					//or the end of obj1 is before the beginning of obj2
					//we have found a separate axis. No collision.
					collision = false;
					break;
				}

				//move on to the next dimension
				++Li1;
				++Li2;
				++Di1;
				++Di2;
			}

			return collision;
		}

		bool debug;

		bool TestObjects(Object* O1, Object* O2){

			//Determine which algorithm we need
			int Complexity;
			if(O1->GetComplexity() > O2->GetComplexity()){
				Complexity = O1->GetComplexity();
			}else{
				Complexity = O2->GetComplexity();
			}

			//use the appropriate algorithm
			bool collision = false;
			if(Complexity == COMP_AABB){
				//typecast to our more specific AABB structure.
				collision = TestAABB((AABB*)O1, (AABB*)O2);
			}

			return collision;
		}
};

#endif
