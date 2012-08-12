

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

	void rotateY(float amount)
	{
		Vector target = m_target;
		Vector right = m_right;

		amount /=57.2957795f; // convert degrees to radians

		Rotation[0] = (cos(1.5708f + amount) * Rotation[0]) + (cos(amount) * right);
	
	}
}
