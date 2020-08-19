#include "Object.h"

double Object::findIntersection(Ray ray) {
	return 0.0f;
}

Color Object::getColor() {
	return Color(.0f, .0f, .0f, 0);
}

void Object::setColor(float r, float g, float b) {
	// empty body
}

glm::vec3 Object::getNormal(glm::vec3 point) {
	return glm::vec3(.0f);
}

materialType Object::getMaterialType() {
	// empty body
}
