#include "Plane.h"

Plane::Plane() {
	this->normal = glm::vec3(1.0f, 0.0f, 0.0f);
	this->color = Color(.5f, .5f, .5f, 0);
	this->center = glm::vec3(.0f);
}

Plane::Plane(glm::vec3 normal, glm::vec3 planeCenter, Color pColor, materialType mat) {
	this->normal = normalize(normal);
	this->color = pColor;
	this->center = planeCenter;
	this->material = mat;
}

bool Plane::findIntersection(glm::vec3 orig, glm::vec3 dir, float& tNear, int& index, glm::vec2& uv) const {
	double t = FLT_MAX;

	// Check if ray is || to plane 
	// i.e Denominator == 0
	float denom = dot(dir, this->normal);
	if (denom == 0) {
		return -1.0f;
	}

	// this->center is the "center" of the plane, 
	// i.e. an Abitrary pt A on the plane
	float numer = dot(this->center - dir, this->normal);
	t = numer / denom;
	
	// check distance is within bounds
	// Also prevents Shadow Acne
	if (t <= T_MIN_VAL || t >= T_MAX_VAL) {
		return -1.0f;
	}

	return t;
}

void Plane::getSurfaceProperties(const glm::vec3& P, 
	const glm::vec3& I, const int& index, 
	const glm::vec2& uv, glm::vec3& N, 
	glm::vec2& st)
{	
	N = normal;
}

materialType Plane::getMaterialType()
{
	return material;
}

glm::vec3 Plane::getNormal(glm::vec3 point) {
	return normal;
}

glm::vec3 Plane::getPlaneCenter() {
	return center;
}

Color Plane::getColor() {
	return color;
}

void Plane::setColor(float r, float g, float b) {
	this->color.setColorR(r);
	this->color.setColorG(g);
	this->color.setColorB(b);
}