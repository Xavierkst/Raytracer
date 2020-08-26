#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "Object.h"

class Rect : public Object {
private:

	glm::vec3 corner;
	glm::vec3 edge_1;
	glm::vec3 edge_2;
	Color color;
	glm::vec3 normal;

public: 
	Rect();

	Rect(glm::vec3 c, glm::vec3 e_a, glm::vec3 e_b, Color col, materialType mat);

	bool findIntersection(glm::vec3 orig, glm::vec3 dir,
		float& tNear, int& index, glm::vec2& uv) const;

	Color getColor();
	void setColor(float r, float g, float b);
	glm::vec3 getNormal(glm::vec3 point);

	void getSurfaceProperties(const glm::vec3& P,
		const glm::vec3& I, const int& index,
		const glm::vec2& uv, glm::vec3& N,
		glm::vec2& st);

	materialType getMaterialType();

};

#endif