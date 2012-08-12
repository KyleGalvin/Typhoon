

class camera : public Object{

	Coordinate Up;
	Coordinate Target;

	camera(){
			//convention: XYZ maps to element 012 respectively

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
		return vec;
	}

	void rotateX(float amount)
	{
		Coordinate tempTarget = Target;
		Coordinate tempUp = Up;

		amount /=57.2957795f; // convert degrees to radians

	//	Target[0] = (cos(1.5708f + amount) * tempTarget[0]) + (cos(amount) * tempUp[0]);
		Target[1] = (cos(1.5708f + amount) * tempTarget[1]) + (cos(amount) * tempUp[1]);
		Target[2] = (cos(1.5708f + amount) * tempTarget[2]) + (cos(amount) * tempUp[2]);
	//	Up[0] = (cos(amount) * tempTarget[0]) + (cos(1.5708f - amount) * tempUp[0]);
		Up[1] = (cos(amount) * tempTarget[1]) + (cos(1.5708f - amount) * tempUp[1]);
		Up[2] = (cos(amount) * tempTarget[2]) + (cos(1.5708f - amount) * tempUp[2]);

		Normalize(Up);
		Normalize(Target);	
	}

	void rotateY(float amount)
	{
		Coordinate tempTarget = Target;
		Coordinate tempUp = Up;

		amount /=57.2957795f; // convert degrees to radians

		Target[0] = (cos(1.5708f + amount) * tempTarget[0]) + (cos(amount) * tempUp[0]);
	//	Target[1] = (cos(1.5708f + amount) * tempTarget[1]) + (cos(amount) * tempUp[1]);
		Target[2] = (cos(1.5708f + amount) * tempTarget[2]) + (cos(amount) * tempUp[2]);
		Up[0] = (cos(amount) * tempTarget[0]) + (cos(1.5708f - amount) * tempUp[0]);
	//	Up[1] = (cos(amount) * tempTarget[1]) + (cos(1.5708f - amount) * tempUp[1]);
		Up[2] = (cos(amount) * tempTarget[2]) + (cos(1.5708f - amount) * tempUp[2]);

		Normalize(Up);
		Normalize(Target);	
	}

	void rotateZ(float amount)
	{
		Coordinate tempTarget = Target;
		Coordinate tempUp = Up;

		amount /=57.2957795f; // convert degrees to radians

		Target[0] = (cos(1.5708f + amount) * tempTarget[0]) + (cos(amount) * tempUp[0]);
		Target[1] = (cos(1.5708f + amount) * tempTarget[1]) + (cos(amount) * tempUp[1]);
	//	Target[2] = (cos(1.5708f + amount) * tempTarget[2]) + (cos(amount) * tempUp[2]);
		Up[0] = (cos(amount) * tempTarget[0]) + (cos(1.5708f - amount) * tempUp[0]);
		Up[1] = (cos(amount) * tempTarget[1]) + (cos(1.5708f - amount) * tempUp[1]);
	//	Up[2] = (cos(amount) * tempTarget[2]) + (cos(1.5708f - amount) * tempUp[2]);

		Normalize(Up);
		Normalize(Target);	
	}
}
