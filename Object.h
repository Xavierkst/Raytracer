#ifndef _OBJECT_H_
#define _OBJECT_H_


#include "Ray.h"
#include "Color.h"

enum materialType {
	REFLECTION_AND_REFRACTION, 
	REFLECTION, 
	DIFFUSE_AND_GLOSSY, 
	DIFFUSE_AND_GLOSSY_AND_REFLECTION, 
	DIFFUSE,
	LIGHT
};

class Object {
private:
	
public:
	// objects by default have ior 1.3 and
	// diffuse and glossy material 
	Object();

	float ior;
	float kd, ks;
	float phongExponent;
	materialType material;

	virtual bool findIntersection(glm::vec3 orig, glm::vec3 dir,
		float& tNear, int& index, glm::vec2& uv) const;

	virtual Color getColor();
	virtual void setColor(float r, float g, float b);
	virtual glm::vec3 getNormal(glm::vec3 point);

	virtual void getSurfaceProperties(const glm::vec3& P, const glm::vec3 orig,
		const glm::vec3& I, const int& index,
		const glm::vec2& uv, glm::vec3& N,
		glm::vec2& st);

	virtual materialType getMaterialType();

};

#endif