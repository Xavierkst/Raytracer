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
	Sphere(glm::vec3 origin, float r, Color c, float refractIdx, materialType mat);


	glm::vec3 getSpherePos();
	Color getColor();
	void setColor(float r, float g, float b);
	float getSphereRadius();
	glm::vec3 getNormal(glm::vec3 point);

	// Calculates the normal at the intersection point of 
	// the surface and stores in N--the normal (other variables unused)
	void getSurfaceProperties(const glm::vec3& P,
		const glm::vec3& I, const int& index,
		const glm::vec2& uv, glm::vec3& N,
		glm::vec2& st);

	bool findIntersection(glm::vec3 orig, glm::vec3 dir,
		float& tNear, int& index, glm::vec2& uv) const;

	bool calcDistance(float& a, float& b, float& c, float& t0, float& t1) const;


	materialType getMaterialType();
};

#endif