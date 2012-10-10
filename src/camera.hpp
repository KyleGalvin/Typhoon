#include "objectstructs.hpp"
#include "math.h"

class camera : public Object{

	public:


	void step(){
		vector< shared_ptr< Force > >::iterator i;
			
			Coordinate NetForce;
			NetForce.push_back(0);
			NetForce.push_back(0);
			NetForce.push_back(0);

		for(i = Forces.begin() ; i != Forces.end() ; i++){
			NetForce[0] += (*((*i)->direction))[0] * (*i)->magnitude;
			NetForce[1] += (*((*i)->direction))[1] * (*i)->magnitude;
			NetForce[2] += (*((*i)->direction))[2] * (*i)->magnitude;
		}
	
		calculateAcceleration(NetForce);

		xVel += xAcc;	
		yVel += yAcc;	
		zVel += zAcc;	
		translateX(xVel);
		translateY(yVel);
		translateZ(zVel);
	}




	camera(){
			//convention: XYZ maps to element 012 respectively

			//x-axis
			shared_ptr<Coordinate> R (new Coordinate());
			R->push_back(1);
			R->push_back(0);
			R->push_back(0);
			Right = R;

			//camera looks down the z-axis
			shared_ptr<Coordinate> T (new Coordinate());
			T->push_back(0);
			T->push_back(0);
			T->push_back(1);
			Target=T;

			//y-axis at the top
			shared_ptr<Coordinate> U (new Coordinate());
			U->push_back(0);
			U->push_back(1);
			U->push_back(0);
			Up=U;
			
			//camera is a point...
			Dimensions.push_back(0);
			Dimensions.push_back(0);
			Dimensions.push_back(0);
			//...at the origin...
			Location.push_back(0);
			Location.push_back(0);
			Location.push_back(0);
			//..with no rotation
			Rotation.push_back(0);
			Rotation.push_back(0);
			Rotation.push_back(0);
		
			mass = 1;
			friction = 0.3;
			maxSpeed=10;

			xVel = 0;
			yVel = 0;
			zVel = 0;

			xAcc = 0;
			yAcc = 0;
			zAcc = 0;
	}

};
