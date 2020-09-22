#include "Sphere.h"

Sphere::Sphere() {
	radius = 1.0f;
	sphereOrig = glm::vec3(0.0f, 1.0f, 5.0f);
	color = Color(0.5f, .5f, .5f, 0);
	bbox.minBounds = sphereOrig + glm::vec3(-1.0f, -1.0f, 1.0f);
	bbox.maxBounds = sphereOrig + glm::vec3(1.0f, 1.0f, -1.0f);
}

Sphere::Sphere(glm::vec3 origin, float r, Color c, 
	float refractIdx, materialType mat) {

	radius = r;
	sphereOrig = origin;
	color = c;
	material = mat;
	ior = refractIdx;
	// Opaque objects have infinite idx of refrac
	if (mat == REFLECTION ||
		mat == DIFFUSE_AND_GLOSSY ||
		mat == DIFFUSE_AND_GLOSSY_AND_REFLECTION) {
		ior = FLT_MAX;
	}
	// build routine for bounding box min, max 
	// note: the min is in the front top left
	// max is in the back bottom right 
	bbox.minBounds = origin + glm::vec3(-r, -r, r);
	bbox.maxBounds = origin + glm::vec3(r, r, -r);
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

void Sphere::getSurfaceProperties(const glm::vec3& P, 
	const glm::vec3 orig, const glm::vec3& I, const int& index, 
	const glm::vec2& uv, glm::vec3& N, glm::vec2& st) {

	N = normalize(P - getSpherePos());
}

// returns true if this particular object/surface is
// intersected. 
// Stores distance from origin to hitpoint in tNear
bool Sphere::findIntersection(glm::vec3 orig, 
	glm::vec3 dir, float& tNear, 
	int& index, glm::vec2& uv) const {

	float t0, t1;
	glm::vec3 R = orig - this->sphereOrig;
	float a = dot(dir, dir);
	float b = 2 * dot(dir, R);
	float c = dot(R, R) - (this->radius * this->radius);

	// if quadratic is true, then there are solutions (either 1 or 2)
	// if calc distance false, no solutions
	if (!calcDistance(a, b, c, t0, t1)) {
		return false;
	}
	if (t0 < 0.0f) t0 = t1;
	if (t0 < 0.0f) return false;
	tNear = t0;

	return true;
}

bool Sphere::calcDistance(float& a, float& b, float& c, 
	float& t0, float& t1) const {
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
		return false;
	}
	
	float rootDiscriminant = sqrt(discriminant);
	t0 = (-b - rootDiscriminant) / (2.0f * a);
	t1 = (-b + rootDiscriminant) / (2.0f * a);

	// we dont know if t0 or t1 are negative values
	// after +- with -b, we'll check in findIntersection()
	// we return true to mean that we have 2 solutions, but not sure
	// if any real solutions
	if (t0 > t1) {
		// swap t0, t1
		float temp = t0; 
		t1 = t0;
		t0 = temp;
	}
	return true;
}

materialType Sphere::getMaterialType() {
	return material;
}

