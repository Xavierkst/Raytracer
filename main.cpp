#include <iostream>
#include <string>
#include <vector>
#include "utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "Object.h"
#include "Plane.h"
#include "Ray.h"
#include "Object.h"

#include "windows.h"
#define _CRTDBG_MAP_ALLOC //to get more details
#include <stdlib.h>  
#include <crtdbg.h>   //for malloc and free

struct RGBColor {
	double r, g, b; 
};

struct Options {
	int width;
	int height;

	float fov;
	float aspectRatio;
	glm::vec3 backgroundColor;
	uint8_t maxDepth;
};

void writeImage(std::string fileName, float exposure, float gamma, RGBColor* pixelData, int width, int height) {
	std::vector<unsigned char> imageData(width * height * 4);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			RGBColor pixelColor = pixelData[x + y * width];
			/*pixelColor.gammaCorrection(exposure, gamma);
			pixelColor.clamp();*/

			int index = 4 * (x + y * width);
			imageData[index + 0] = (unsigned char)(pixelColor.r * 255.0f);
			imageData[index + 1] = (unsigned char)(pixelColor.g * 255.0f);
			imageData[index + 2] = (unsigned char)(pixelColor.b * 255.0f);
			imageData[index + 3] = 255; // alpha channel
		}
	}
	std::cout << imageData.size() << std::endl;
	std::cout << height << " " << width << std::endl;
	resultToPNG(fileName, width, height, imageData);
}

// Returns the index of the closest object
int findClosestObjIndex(std::vector<double>& intersections) {
	double max = FLT_MAX;
	int closestIdx = 0;
	if (intersections.size() == 0) {
		return -1;
	}
	// find the min dist in the array
	for (int k = 0; k < intersections.size(); k++) {
		if (intersections[k] > T_MIN_VAL && intersections[k] < max) {
			max = intersections[k];
			closestIdx = k;
		}
	}
	// if there is no intersection that is positive
	if (max == FLT_MAX) {
		return -1;
	}
	else {
		return closestIdx;
	}
}

int main(int argc, char* argv[]) {
	_CrtMemState sOld;
	_CrtMemState sNew;
	_CrtMemState sDiff;
	_CrtMemCheckpoint(&sOld); //take a snapchot

	std::cout << "Rendering... 123" << std::endl;
	Options options; 
	options.width = 640;
	options.height= 480;
	options.aspectRatio = (float)options.width / (float)options.height;
	options.fov = M_PI * (90.0f / 180.0f); 

	// initializing all pixels (color buffer)
	RGBColor* colorBuffer = new RGBColor[options.width*options.height];
	for (int i = 0; i < options.width*options.height; i++) {
		colorBuffer[i].r = 1.0;
		colorBuffer[i].g = 1.0;
		colorBuffer[i].b = 0.0;
	}

	// Colors
	Color whiteLight(1.0f, 1.0f, 1.0f, 0.0f);
	Color maroon(.5f, .25f, .25f, 0);
	Light theLight(glm::vec3(0.0f, 5.0f, 0.0f), whiteLight);
	Color prettyGreen(0.5f, 1.0f, 0.5f, 0.3f);
	Color gray(.5f, .5f, .5f, .0f);
	Color black(.0f, .0f, .0f, .0f);

	// Objects
	Sphere scene_sphere(glm::vec3(.0f, .0f, 4.0f), 1.0f, prettyGreen);
	Plane plane(glm::vec3(.0f, 1.0f, .0f), glm::vec3(.0f, -1.0f, .0f), maroon);

	// Generating Camera  
	glm::vec3 cameraPos(.0f, .0f, 0.0f);
	glm::vec3 cameraForward(.0f, .0f, 1.0f);
	glm::vec3 cameraReferUp(.0f, 1.0f, .0f);
	glm::vec3 cameraRight(1.0f, .0f, .0f);
	Camera cam(cameraPos, cameraForward, cameraReferUp);

	// Fill list of scene objects
	std::vector<Object*> sceneObjects;
	sceneObjects.push_back(&scene_sphere);
	sceneObjects.push_back(&plane);

	// world space to screen space
	float alpha, beta;
	glm::vec3 rayDir, rayOrigin;

	for (int y = 0; y < options.height; y++) {
		for (int x = 0; x < options.width; x++) {
			alpha = ( (2 * (x + 0.5)/ (float)options.width) - 1.0f) 
				* options.aspectRatio * tan(options.fov/2);
			beta = (1 - (2 * (y + 0.5) / (float) options.height)) * tan(options.fov / 2);

			rayDir = normalize(glm::vec3(alpha, beta, .0f) + cam.getCamDir());
			rayOrigin = cameraPos;
			Ray camRay(rayOrigin, rayDir);
			
			std::vector<double> intersections;
			// store all intersect distances (even if they are negative, we'll weed
			// them out later in findClosestObjIndeX() function)
			for (int idx = 0; idx < sceneObjects.size(); idx++) {
				float d = sceneObjects[idx]->findIntersection(camRay);
				intersections.push_back(d);
			}
			// pixel color
			Color pixelColor;
			// find the index of closest object, get its color
			int closestIndex = findClosestObjIndex(intersections);
			// negative index means no intersection, use default color
			// move on to next pixel
			if (closestIndex < 0) {
				continue;
			}
			else {
				pixelColor = sceneObjects[closestIndex]->getColor();
				colorBuffer[x + y * options.width].r = pixelColor.getColorR();
				colorBuffer[x + y * options.width].g = pixelColor.getColorG();
				colorBuffer[x + y * options.width].b = pixelColor.getColorB();
			}
			// TODO: cast generated rays into the scene 
			// using a pointer to a list of Objects, call intersection function
			// continue this url: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
		}
	}

	std::string fileName = "testFile.jpg";
	writeImage(fileName, 1.0, 2.2, colorBuffer, options.width, options.height);

	// Free memory
	delete[] colorBuffer;
	//delete sceneObjects[0];
	//delete sceneObjects[1];
	while (!sceneObjects.empty()) {
		sceneObjects.pop_back();
	}
	
	sceneObjects.clear();

	_CrtMemCheckpoint(&sNew); //take a snapchot 
	if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
	{
		OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
		_CrtMemDumpStatistics(&sDiff);
		OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
		_CrtMemDumpAllObjectsSince(&sOld);
		OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
		_CrtDumpMemoryLeaks();
	}
	return 0;
}