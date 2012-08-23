#include "objectstructs.hpp"
#include "math.h"

class camera : public Object{

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


	shared_ptr<Force>  addForce(shared_ptr<Coordinate> unitVector, float magnitude){

		shared_ptr<Force> f (new Force());
		f->magnitude = magnitude;	
		f->direction = unitVector;	

		Forces.push_back(f);
		return f;
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

	void Normalize(shared_ptr<Coordinate> vec){
		//normalize!
		float len = sqrt(((*vec)[0] * (*vec)[0])+((*vec)[1] * (*vec)[1])+((*vec)[2] * (*vec)[2]));
		(*vec)[0]/=len;
		(*vec)[1]/=len;
		(*vec)[2]/=len;
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
};
