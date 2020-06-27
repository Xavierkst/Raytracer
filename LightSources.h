#ifndef _LIGHT_SOURCES_H_
#define _LIGHT_SOURCES_H_

#include "Color.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class LightSources {
private:

public:
	virtual Color getLightColor();
	virtual glm::vec3 getLightPos();
};

#endif