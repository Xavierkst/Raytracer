#ifndef _PLANE_H_
#define _PLANE_H_

#include "Color.h"
#include "Object.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Plane : public Object {
private:
	glm::vec3 normal;
	Color color;
	// distance of the plane's center from world origin
	glm::vec3 center;
public:
	Plane();
	Plane(glm::vec3 normal, glm::vec3 planeCenter, Color pColor);

	glm::vec3 getNormal();
	Color getColor();
	glm::vec3 getPlaneCenter(); 
	
	double findIntersection(Ray ray);
};

#endif