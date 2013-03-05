#include <vector>
#include <iostream>
std::vector<float> scale(float scalar, std::vector<float> a){
	for(std::vector<float>::iterator i = a.begin(); i!=a.end();i++){
		(*i)*=scalar;
	}
	return a;
}

std::vector<float> average(std::vector< std::vector<float> > list){

	//initialize our return value
	std::vector<float> result;
	for(int i = 0;i<list[0].size();i++){
		result.push_back(0.0);
	}

	//sum each vector in the list into our result
	std::vector< std::vector<float> >::iterator listIter = list.begin();
	while(listIter != list.end()){
		for(int i=0;i<(*listIter).size();i++){
			result[i] += (*listIter)[i];
		}				
		listIter++;
	}

	//divide result by number of vectors in list
	for(int i = 0; i<list[0].size();i++){
		result[i]/=list.size();
	}

	return result;
}
std::vector<float> translate(std::vector<float> a, std::vector<float> b){
	std::vector<float>::iterator j = b.begin();
	for(std::vector<float>::iterator i = a.begin(); i!=a.end();i++){
		(*i)+=(*j);
		j++;
	}
	return a;
}

float vLength(std::vector<float> v){
	float intermediate = 0;
	for(std::vector<float>::iterator i = v.begin(); i != v.end(); i++){
		intermediate += pow((*i),2);
	}
	return sqrt(intermediate);

}
std::vector<float> normalize(std::vector<float> v){
	float length = vLength(v);
	for(std::vector<float>::iterator i = v.begin(); i < v.end(); i++){
		(*i) /= length;
	}

	return v;
}
std::vector<float> absTranslate(float scalar, std::vector<float> a){
	for(std::vector<float>::iterator i = a.begin(); i!=a.end();i++){
		(*i)+=scalar;
	}
	return a;
}

std::vector<float> delta(std::vector<float> a, std::vector<float> b){
	std::vector<float>::iterator j = b.begin();
	for(std::vector<float>::iterator i = a.begin(); i!=a.end();i++){
		(*i)-=(*j);
		(*i)=abs(*i);
		j++;
	}
	return a;
}

float absDistance(std::vector<float> a, std::vector<float> b){
	std::vector<float>::iterator i = a.begin();
	std::vector<float>::iterator j = b.begin();

	float intermediate = 0;

	while(i!=a.end()){
		intermediate += pow((*i)-(*j),2);
		i++;
		j++;
	}
	return sqrt(intermediate);
}

std::vector<float> midpoint(std::vector<float> a, std::vector<float> b){
	std::vector<float> result;
	float min;
	//we assume a and b are of same length. If someone breaks this rule, it's their problem to debug.

	//j iterator offset mirrors i iterator offset, but points to other input std::vector<float>
	std::vector<float>::iterator j = b.begin();
	for(std::vector<float>::iterator i=a.begin(); i!=a.end();i++){
		if((*i)>(*j))
			min = *j;
		else
			min = *i;
		result.push_back(min+(sqrt(pow((*i)-(*j),2))/2.0));
		j++;
	}

	return result;
}

std::vector<float> repel(std::vector<float> obstacle, std::vector<float> target, float percent){
	//return (to-from)*percent;
	//repulsive force modelled after electrostatic repulsion of same-charge particles (charge divided by delta-distance squared)
	float repulsiveForce = 0.025;
	std::vector<float> result;

	std::vector<float>::iterator j = target.begin();
	for(std::vector<float>::iterator i = obstacle.begin();i!=obstacle.end();i++){
		result.push_back((*j)-((repulsiveForce*percent) / (1+sqrt(pow((*i)-(*j),2)))));

		j++;
	}

	return result;
}

//this works on a normalized map.
std::vector<float> scaleWRTRadiusFromCenter(std::vector<float> training, std::vector<float> winner, float percent){
	//iterate down both vectors to get magnitude of euclidian distance between the two
	float magnitudeT=0;
	float magnitudeW=0;

	float magnitude = (-1)*sqrt((1-vLength(training))*(1-vLength(winner)));
	float k = /*1 -*/ magnitude; //TSP and genetic algorithsm, equation 4
	float c = 1 / sqrt(1- (2*percent * (1-percent)* k));

	std::vector<float> result;
	for(std::vector<float>::iterator j = winner.begin(); j!=winner.end();j++){
		result.push_back((*j) * c);
	}
	return result;
}


std::vector<float> attract(std::vector<float> to, std::vector<float> from, float percent){
	//return (to-from)*percent;

	std::vector<float> result;

	std::vector<float>::iterator j = from.begin();
	for(std::vector<float>::iterator i = to.begin();i!=to.end();i++){
		result.push_back((*j)+( percent * ((*i)-(*j)) ));
		j++;
	}

	return result;
}
