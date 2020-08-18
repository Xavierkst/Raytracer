#ifndef _RAY_H
#define _RAY_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define T_MIN_VAL 0.001f
#define T_MAX_VAL 1.0e29f

class Ray {
private:
	// origin of ray, and time 
	float t;
	glm::vec3 rayOrig, rayDir;

public:
	glm::vec3 getRayOrig();
	glm::vec3 getRayDir();

	Ray();
	// Constructor for a ray 
	Ray(glm::vec3 origin, glm::vec3 direction);

	//void intersect(glm::vec3 )
};

#endif