#include "Object.h"


Object::Object() : ior(1.3f), material(DIFFUSE_AND_GLOSSY), 
	kd(0.8f), ks(0.6f), phongExponent(15)
{
}

bool Object::findIntersection(glm::vec3 orig,
	glm::vec3 dir, float& tNear, 
	int& index, glm::vec2& uv) const
{
	return false;
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

void Object::getSurfaceProperties(const glm::vec3& P, 
	const glm::vec3& I, const int& index, 
	const glm::vec2& uv, glm::vec3& N, 
	glm::vec2& st)
{
}

materialType Object::getMaterialType() {
	// empty body
	return DIFFUSE;
}
