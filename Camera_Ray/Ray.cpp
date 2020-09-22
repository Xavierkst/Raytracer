#include "Ray.h"

Ray::Ray() {
	rayOrig = glm::vec3(0.0f);
	rayDir = glm::vec3(.0f, .0f, 1.0f);
	t = FLT_MAX;
}

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
	rayOrig = origin;
	rayDir = direction;
	t = FLT_MAX;
}

glm::vec3 Ray::getRayOrig() {
	return rayOrig;
}

glm::vec3 Ray::getRayDir() {
	return rayDir;
}
