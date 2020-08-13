#include "Plane.h"

Plane::Plane() {
	this->normal = glm::vec3(1.0f, 0.0f, 0.0f);
	this->color = Color(.5f, .5f, .5f, 0);
	this->center = glm::vec3(.0f);
}

Plane::Plane(glm::vec3 normal, glm::vec3 planeCenter, Color pColor) {
	this->normal = normalize(normal);
	this->color = pColor;
	this->center = planeCenter;
}

double Plane::findIntersection(Ray ray) {
	double t = FLT_MAX;

	// Check if ray is || to plane 
	// i.e Denominator == 0
	float denom = dot(ray.getRayDir(), this->normal);
	if (denom == 0) {
		return -1.0f;
	}

	// Abitrary pt A on the plane
	float numer = dot(this->center - ray.getRayOrig(), this->normal);
	t = numer / denom;
	
	// check distance is within bounds
	// Also prevents Shadow Acne
	if (t <= T_MIN_VAL || t >= T_MAX_VAL) {
		return -1.0f;
	}

	return t;
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