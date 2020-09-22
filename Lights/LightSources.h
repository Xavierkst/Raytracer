#ifndef _LIGHT_SOURCES_H_
#define _LIGHT_SOURCES_H_

#include "../Color.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum lightType { POINT_LIGHT, AREA_LIGHT };


class LightSources {
private:

public:
	virtual Color getLightColor();
	virtual glm::vec3 getLightPos();
	virtual lightType getLightType();

	virtual glm::vec3 getEdgeA();
	virtual glm::vec3 getEdgeB();
	
};

#endif