#ifndef _OBJECT_H_
#define _OBJECT_H_


#include "Ray.h"
#include "Color.h"

class Object {
private:
	
public:
	virtual double findIntersection(Ray ray);

	virtual Color getColor();

	virtual glm::vec3 getNormal();
};

#endif