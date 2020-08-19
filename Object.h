#ifndef _OBJECT_H_
#define _OBJECT_H_


#include "Ray.h"
#include "Color.h"

enum materialType {
	REFLECTION_AND_REFRACTION, REFLECTION, DIFFUSE_AND_GLOSSY, DIFFUSE
};

class Object {
private:
	
public:
	virtual double findIntersection(Ray ray);

	virtual Color getColor();
	virtual void setColor(float r, float g, float b);
	virtual glm::vec3 getNormal(glm::vec3 point);
	virtual materialType getMaterialType();
};

#endif