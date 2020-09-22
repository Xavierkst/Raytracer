#include "AccelerationStructure.h"

AccelerationStructure::AccelerationStructure(std::vector<Object*>& objectList) :
	objects(/*std::move(*/objectList/*)*/) {
	// both reference the same vector of objects 
}

AccelerationStructure::~AccelerationStructure()
{
}

bool AccelerationStructure::findIntersection()
{
	return false;
}