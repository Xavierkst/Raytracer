#include "Sphere.h"

Sphere::Sphere() {
	radius = 1.0f;
	sphereOrig = glm::vec3(0.0f, 1.0f, 5.0f);
	color = Color(0.5f, .5f, .5f, 0);
}

Sphere::Sphere(glm::vec3 origin, float r, Color c) {
	radius = r;
	sphereOrig = origin;
	color = c;
}

glm::vec3 Sphere::getSpherePos() {
	return sphereOrig;
}

Color Sphere::getColor() {
	return color;
}

void Sphere::setColor(float r, float g, float b) {
	this->color.setColorR(r);
	this->color.setColorG(g);
	this->color.setColorB(b);
}

float Sphere::getSphereRadius() {
	return radius;
}

glm::vec3 Sphere::getNormal(glm::vec3 point)
{
	return normalize(point - this->getSpherePos());
}

double Sphere::findIntersection(Ray ray) {
	glm::vec3 R = ray.getRayOrig() - this->sphereOrig;
	float a = dot(ray.getRayDir(), ray.getRayDir());
	float b = 2 * dot(ray.getRayDir(), R);
	float c = dot(R, R) - (this->radius * this->radius);

	return calcDistance(a, b, c);
}

double Sphere::calcDistance(float a, float b, float c) {
	// 4 cases:
	// 1. Ray grazes sphere (tangent): 2 real tVal, both same
	// 2. Ray origin starts from inside sphere: 1 -ve, 1 +ve tVal
	// take +ve
	// 3. Ray completely misses: 2 complex values, take none
	// 4. Ray intersects sphere at 2 non-tangent points: 2 +ve tVal
	// take smaller of 2
	float discriminant = (b*b) - (4*a*c);
	// if t is complex (negative)
	if (discriminant < 0.0f) {
		return -1.0f;
	}
	
	float rootDiscriminant = sqrt(discriminant);
	float t1 = (-b - rootDiscriminant) / (2 * a);
	float t2 = (-b + rootDiscriminant) / (2 * a);
	float tVal = -1.0f;
	// Consider cases where b is -ve or +ve,
	// the correct t value is always chosen,
	if (t1 >= T_MIN_VAL) {
		return t1;
	}
	else if (t2 >= T_MIN_VAL) {
		return t2;
	}
	else
		// Ray misses entirely, return -1
		return -1.0f;
}

