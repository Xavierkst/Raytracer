#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <glm/glm.hpp>

#define MAX_RECURSION_DEPTH 8
#define STARTING_DEPTH 0

class Options {
private:

public:
	int width;
	int height;
	// Anti-aliasing depth (default: 1) 
	// 1 pixel, 4 pixels, 9 etc.
	int aaDepth;
	float fov;
	float aspectRatio;
	float ambientLight;
	Color backgroundColor;
	float bias;

	float sampleNum;

	bool softShadows;
	// default constructor
	Options() {
		softShadows = true;
		sampleNum = 4;
		aaDepth = 1;
		width = 1080;
		height = 720;
		aspectRatio = (float)width / (float)height;
		fov = M_PI * (90.0f / 180.0f);
		ambientLight = 0.4f;
		bias = 0.01f;
		backgroundColor = Color(201.0f / 255.0f, 226.0f / 255.0f, 255.0f / 255.0f, .0f);
	}
};
#endif 