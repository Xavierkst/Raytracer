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
	
	float bias;
	// default constructor
	Options() {
		width = 1080;
		height = 720;
		aspectRatio = (float)width / (float)height;
		fov = M_PI * (90.0f / 180.0f);
		ambientLight = 0.2f;
		bias = 0.001f;
	}
};
#endif 