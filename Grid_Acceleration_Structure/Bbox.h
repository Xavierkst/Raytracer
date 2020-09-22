#ifndef _BBOX_H_
#define _BBOX_H_

#include <glm/glm.hpp>
#include <math.h>
#define _USE_MATH_DEFINES
#include <algorithm>

class Bbox {
private:

public:
	// min and max bounds of the Grid1
	glm::vec3 minBounds;
	glm::vec3 maxBounds;
	
	Bbox();
	// needs to initialize the min and max to inf & -inf
	Bbox(glm::vec3 min, glm::vec3 max);
	
	// increases the grid's size accordingly by merging
	// all the scene's object's bounding boxes
	Bbox& extendBy(glm::vec3& Pt);

	// Ray box intersection routine same as in Box class
	bool findIntersection(glm::vec3 orig, glm::vec3 dir, float& tNear);

	// get centroid function?
	glm::vec3 getCentroid();

};

#endif