#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Color.h"
#include "Object.h"

class Sphere : public Object {
private:
	float radius;
	glm::vec3 sphereOrig;
	Color color; 
public:

	Sphere();
	Sphere(glm::vec3 origin, float r, Color c);

	glm::vec3 getSpherePos();
	Color getColor();
	float getSphereRadius();
	glm::vec3 getNormal(glm::vec3 point);
	double findIntersection(Ray ray);
	double calcDistance(float a, float b, float c);
};

#endif