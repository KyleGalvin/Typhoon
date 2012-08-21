#include "objectstructs.hpp"
#include "math.h"

class camera : public Object{

	public:
	Coordinate Up;
	Coordinate Target;
	Coordinate Right;

	camera(){
			//convention: XYZ maps to element 012 respectively

			//x-axis
			Right.push_back(1);
			Right.push_back(0);
			Right.push_back(0);

			//camera looks down the z-axis
			Target.push_back(0);
			Target.push_back(0);
			Target.push_back(1);

			//y-axis at the top
			Up.push_back(0);
			Up.push_back(1);
			Up.push_back(0);
			
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
	}

	Coordinate Normalize(Coordinate vec){
		//normalize!
		float len = sqrt((vec[0]*vec[0])+(vec[1]*vec[1])+(vec[2]*vec[2]));
		vec[0]/=len;
		vec[1]/=len;
		vec[2]/=len;
		return vec;
	}

	void rotateX(float amount)
	{
		Coordinate tempTarget = Target;
		Coordinate tempUp = Up;

		amount /=57.2957795f; // convert degrees to radians
		
		Target[0] = (cos(amount) * tempTarget[0]) - (sin(amount) * tempUp[0]);
		Target[1] = (cos(amount) * tempTarget[1]) - (sin(amount) * tempUp[1]);
		Target[2] = (cos(amount) * tempTarget[2]) - (sin(amount) * tempUp[2]);

		Up[0] = (cos(amount) * tempUp[0]) + (sin(amount) * tempTarget[0]);
		Up[1] = (cos(amount) * tempUp[1]) + (sin(amount) * tempTarget[1]);
		Up[2] = (cos(amount) * tempUp[2]) + (sin(amount) * tempTarget[2]);

		Target = Normalize(Target);	
		Up = Normalize(Up);
	}

	void rotateY(float amount)
	{
		Coordinate tempTarget = Target;
		Coordinate tempRight = Right;

		amount /=57.2957795f; // convert degrees to radians

		Target[0] = (cos(amount) * tempTarget[0]) + (sin(amount) * tempRight[0]);
		Target[1] = (cos(amount) * tempTarget[1]) + (sin(amount) * tempRight[1]);
		Target[2] = (cos(amount) * tempTarget[2]) + (sin(amount) * tempRight[2]);

		Right[0] = (cos(amount) * tempRight[0]) - (sin(amount) * tempTarget[0]);
		Right[1] = (cos(amount) * tempRight[1]) - (sin(amount) * tempTarget[1]);
		Right[2] = (cos(amount) * tempRight[2]) - (sin(amount) * tempTarget[2]);

		Right = Normalize(Right);
		Target = Normalize(Target);	
	}

	void rotateZ(float amount)
	{
		Coordinate tempRight = Right;
		Coordinate tempUp = Up;
		
		amount /=57.2957795f; // convert degrees to radians

		Up[0] = (cos(amount) * tempUp[0]) - (sin(amount) * tempRight[0]);
		Up[1] = (cos(amount) * tempUp[1]) - (sin(amount) * tempRight[1]);
		Up[2] = (cos(amount) * tempUp[2]) - (sin(amount) * tempRight[2]);

		Right[0] = (cos(amount) * tempRight[0]) + (sin(amount) * tempUp[0]);
		Right[1] = (cos(amount) * tempRight[1]) + (sin(amount) * tempUp[1]);
		Right[2] = (cos(amount) * tempRight[2]) + (sin(amount) * tempUp[2]);

		Up=Normalize(Up);
		Right=Normalize(Right);	
	}
	
	void translateX(float amount){
		Location[0]+= Right[0]*amount;
	//	Location[1]+= Right[1]*amount;
	//	Location[2]+= Right[2]*amount;
	}

	void translateY(float amount){
	//	Location[0]+= Up[0]*amount;
		Location[1]+= Up[1]*amount;
	//	Location[2]+= Up[2]*amount;
	}

	void translateZ(float amount){
	//	Location[0]+= Target[0]*amount;
	//	Location[1]+= Target[1]*amount;
		Location[2]+= Target[2]*amount;
	}
};
