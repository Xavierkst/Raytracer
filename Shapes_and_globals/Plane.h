#ifndef _PLANE_H_
#define _PLANE_H_

#include "../Lights_Color/Color.h"
#include "Object.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Plane : public Object {
private:
	glm::vec3 normal;
	Color color;
	// distance of the plane's center from world origin
	glm::vec3 center;

public:
	Plane();
	Plane(glm::vec3 normal, glm::vec3 planeCenter, Color pColor, materialType mat);

	glm::vec3 getNormal(glm::vec3 point);
	Color getColor();
	void setColor(float r, float g, float b);
	glm::vec3 getPlaneCenter(); 

	bool findIntersection(glm::vec3 orig, glm::vec3 dir,
		float& tNear, int& index, glm::vec2& uv) const;

	void getSurfaceProperties(const glm::vec3& P, const glm::vec3 orig,
		const glm::vec3& I, const int& index,
		const glm::vec2& uv, glm::vec3& N,
		glm::vec2& st);

	materialType getMaterialType();
};

#endif