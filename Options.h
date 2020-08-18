#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <glm/glm.hpp>

class Options {
private:

public:
	int width;
	int height;

	float fov;
	float aspectRatio;
	float ambientLight;
	glm::vec3 backgroundColor;
	uint8_t maxDepth;
};
#endif 