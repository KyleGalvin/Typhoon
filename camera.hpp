#include "objectstructs.hpp"
#include "math.h"

class camera : public Object{

	public:
	Coordinate Up;
	Coordinate Target;
	Coordinate Right;

	Coordinate NetForce;

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

/*	Coordinate calculateFriction(NetForce){
		Coordinate result.push_back();
		
	}*/

	void step(){
		//Coordinate FrictionalDapmenedForce = calculateFriction(NetForce);
		calculateAcceleration(NetForce);

		xVel += xAcc;	
		yVel += yAcc;	
		zVel += zAcc;	
		translateX(xVel);
		translateY(yVel);
		translateZ(zVel);
	}

	void addForce(Coordinate unitVector, float magnitude){
		NetForce[0] += unitVector[0] * magnitude;		
		NetForce[1] += unitVector[1] * magnitude;		
		NetForce[2] += unitVector[2] * magnitude;		
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
		
			NetForce.push_back(0);
			NetForce.push_back(0);
			NetForce.push_back(0);
		
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
		//mod 360 for sane numbers
		int IntAmount = (int) amount;
		int AdjustedIntAmount = IntAmount % 360;
		//save the decimal/fraction
		amount -= IntAmount;
		//add the simplified whole with the fraction
		amount +=AdjustedIntAmount;

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
		//mod 360 for sane numbers
		int IntAmount = (int) amount;
		int AdjustedIntAmount = IntAmount % 360;
		//save the decimal/fraction
		amount -= IntAmount;
		//add the simplified whole with the fraction
		amount +=AdjustedIntAmount;
		
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
		//mod 360 for sane numbers
		int IntAmount = (int) amount;
		int AdjustedIntAmount = IntAmount % 360;
		//save the decimal/fraction
		amount -= IntAmount;
		//add the simplified whole with the fraction
		amount +=AdjustedIntAmount;
		
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
		Location[1]+= Right[1]*amount;
		Location[2]+= Right[2]*amount;
	}

	void translateY(float amount){
		Location[0]+= Up[0]*amount;
		Location[1]+= Up[1]*amount;
		Location[2]+= Up[2]*amount;
	}

	void translateZ(float amount){
		Location[0]+= Target[0]*amount;
		Location[1]+= Target[1]*amount;
		Location[2]+= Target[2]*amount;
	}
};
