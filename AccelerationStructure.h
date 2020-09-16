#ifndef _ACCELERATION_STRUCTURE_H_
#define _ACCELERATION_STRUCTURE_H_

#include <vector>
#include "Object.h"

class AccelerationStructure {
private: 
	

public: 

	AccelerationStructure(std::vector<Object*>& objectList);

	~AccelerationStructure();

	virtual bool findIntersection();

	// vector of meshes/objects  
	// passed into accel structure to iterate thru
	std::vector<Object*> objects;
};
#endif