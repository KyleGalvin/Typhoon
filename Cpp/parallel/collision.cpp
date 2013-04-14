struct objBoundary{
	bool end;
	float location;
	unsigned int ID;
};

struct axialProjection{
	std::list<objBoundary> objFragments;
};

bind(PtrListPtrObj objs){

}

//sort(axialProjection* ap){	
//}
