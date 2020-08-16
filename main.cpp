#include <iostream>
#include <string>
#include <vector>
#include "utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "LightSources.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "Object.h"
#include "Plane.h"
#include "Ray.h"
#include "Object.h"
#include "time.h"

#define MAX_RECURSION_DEPTH 1

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
	float ambientLight;
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

// intersectingRayDir used for finding reflected Ray
// intersectingRayDir i.e. opposite of viewDirection V 
// view direction V points from the object to eye
Color getColorAt(glm::vec3 intersectionPos, glm::vec3 intersectingRayDir,
	std::vector<LightSources*>& sources, std::vector<Object*> objects,
	int closestIdx, float ambientLightConst, int reflectionDepth) {


	if (objects[closestIdx]->getColor().getColorSpecial() == 2.0f) {
		
		int squareTile = floor(intersectionPos.x) + floor(intersectionPos.z);
		if (squareTile % 2 == 0) {
			objects[closestIdx]->setColor(0.0f, 0.0f, 0.0f);
		}
		else {
			objects[closestIdx]->setColor(1.0f, 1.0f, 1.0f);
		}
	}

	// get closest (primary) object color 
	Color finalColor = objects[closestIdx]->getColor() * ambientLightConst;
	glm::vec3 objectNormal = objects[closestIdx]->getNormal(intersectionPos);

	// Reflection rays: 
	// cast secondary rays from the primary intersection 
	// point and do intersection test again with all objects in the scene.
	// Sum up their surface color contributions with the primary object's 
	// surface color (recursively call getColorAt() again) 
	
	// check if object has a shiny/glossy property (means has reflections)
	// i.e. specialValue in range [0,1]
	if (objects[closestIdx]->getColor().getColorSpecial() > 0.0f &&
		objects[closestIdx]->getColor().getColorSpecial() <= 1.0f && reflectionDepth < MAX_RECURSION_DEPTH) {
		// Do intersection test at the point of intersection on the primary obj
		glm::vec3 scalar = 2.0f * objectNormal * dot(intersectingRayDir, objectNormal);
		glm::vec3 reflectionDir = normalize(intersectingRayDir - scalar);
		Ray reflection_ray(intersectionPos, reflectionDir);
		std::vector<double> reflectionArr; 

		for (int j = 0; j < objects.size(); j++) {
			reflectionArr.push_back(objects[j]->findIntersection(reflection_ray));
		}
		int closestRefObjIdx = findClosestObjIndex(reflectionArr);
		// if closest reflected Object index is not -1, the reflected ray
		// hit an object. So we sample the color of the object
		if (closestRefObjIdx != -1) {
			glm::vec3 ref_intersect_pt = intersectionPos + reflectionDir * (float)reflectionArr[closestRefObjIdx];
			finalColor = finalColor + getColorAt(ref_intersect_pt, reflectionDir,
				sources, objects, closestRefObjIdx, ambientLightConst, ++reflectionDepth) *
				objects[closestIdx]->getColor().getColorSpecial();
		}
	}
 	// iterate thru all light sources and sum up their effects
	// on the primary object's surface color 
	for (int k = 0; k < sources.size(); k++) {
		glm::vec3 lightPos = sources[k]->getLightPos();
		glm::vec3 lightDir = normalize(lightPos - intersectionPos);

		// dot product of light and surface normal, use it to multiply with 
		// surface color and light color
		float cos_theta = dot(objects[closestIdx]->getNormal(intersectionPos), lightDir);

		// when angle between normal and light dir is at most 90 deg, light
		// hits the surface, so we attempt to trace to light source. Else, no light 
		// hits it, we darken it with a scalar multip?..
		if (cos_theta > 0) {
			//bool shadowed = false;
			// find distance from intersection pt to light source
			float intersection_to_light_dist = distance(intersectionPos, lightPos);
			// Cast 2ndary rays and do intersection test: 
			Ray shadowRay(intersectionPos, lightDir);
			std::vector<double> intersectionArr;

			// iterate thru all the objects 
			for (int idx = 0; idx < objects.size(); idx++) {
				double d = objects[idx]->findIntersection(shadowRay);
				// collect an array of distance values, including the intersections that 
				// completely miss objects, those are stored in Arr as -ve values
				// which we filter out later in findClosestObjIndex()
				intersectionArr.push_back(d);
			}

			// TEST FOR SHADOWS --
			// check if any object intersects with secondary ray:
			int closestObjIdx = findClosestObjIndex(intersectionArr);
			
			if (closestObjIdx < 0) {
				// NO intersection occured--light is not blocked!
				// add light value to it:
				// sufaceColor * LightColor * dot prod bet. surface normal & light dir
				// Note: closestIdx is the idx of the closest object we hit that we are generating
				// shadow rays from, directed towards the light --> closestIdx != closestObjIdx
				// IMPT: the algorithm we use to do intersection test assumes the light to be 
				// infinitely far away, so the actual light position is actually a light direction
				// i.e. objects can be further from the primary object than the light is and still
				// generate shadow on the primary object
				finalColor = finalColor + objects[closestIdx]->getColor() * 
					sources[k]->getLightColor() * cos_theta;
				
				// account for specular (shininess of object)
				// if material is shiny (has special component -- note shiny is in range [0,1])
				// any value beyond is not considered shiny

			/*	if (finalColor.getColorSpecial() > 0.0f && 
					finalColor.getColorSpecial() <= 1.0f) 
				*/
				if (objects[closestIdx]->getColor().getColorSpecial() > 0.0f &&
					objects[closestIdx]->getColor().getColorSpecial() <= 1.0f) {
					/*glm::vec3 scalar = 2.0f * (intersectingRayDir +
						objectNormal * dot(intersectingRayDir, objectNormal));
					glm::vec3 reflectionDir = normalize(-intersectingRayDir + scalar);*/

					glm::vec3 scalar = 2.0f * objectNormal * dot(lightDir, objectNormal);
					glm::vec3 reflectionDir = normalize(scalar - lightDir);
					// negative of intersectingRayDir == viewDir
					double specularVal = dot(reflectionDir, -intersectingRayDir);
					if (specularVal > 0.0f) {
						// not sure why raised to the 10
						specularVal = pow(specularVal, 10);
						finalColor = finalColor + sources[k]->getLightColor() * 
							specularVal * finalColor.getColorSpecial();
					}
				}
			}
			else {
				// intersection occured: no contribution to finalColor 
				// by this light source, move on to next light source
				continue;
			}
		}
	}
	return finalColor.colorClip();
}

int main(int argc, char* argv[]) {
	// checking for memory leaks
	_CrtMemState sOld;
	_CrtMemState sNew;
	_CrtMemState sDiff;
	_CrtMemCheckpoint(&sOld); //take a snapshot

	// Anti-aliasing depth (default: 1) 
	// (1 == 1 pixel, 2 == 4 pixel colors avged into 1)
	int aaDepth = 5; 

	std::cout << "Rendering... " << std::endl;
	// Setting up image options
	Options options; 
	options.width = 1080;
	options.height= 720;
	options.aspectRatio = (float)options.width / (float)options.height;
	options.fov = M_PI * (90.0f / 180.0f); 
	options.ambientLight = 0.2f;

	// include time
	clock_t t1, t2; 
	t1 = clock();

	// initializing all pixels to default value (color buffer)
	RGBColor* colorBuffer = new RGBColor[options.width*options.height];
	for (int i = 0; i < options.width*options.height; i++) {
		colorBuffer[i].r = 0.0f;
		colorBuffer[i].g = 0.0f;
		colorBuffer[i].b = 0.0f;
	}

	// Colors
	Color whiteLight(1.0f, 1.0f, 1.0f, 0.0f);
	// special value of flooring set to 2
	Color maroon(.5f, .25f, .25f, 0.5f);
	Color white(1.0f, 1.0f, 1.0f, 2.0f);
	Color prettyGreen(0.5f, 1.0f, 0.5f, 0.5f);
	Color gray(.5f, .5f, .5f, .0f);
	Color black(.0f, .0f, .0f, .0f);

	// Lights
	Light theLight(glm::vec3(-7.0f, 5.0f, -3.0f), whiteLight);

	// Objects
	Sphere scene_sphere(glm::vec3(.0f, .0f, 3.0f), 1.0f, prettyGreen);
	Sphere scene_sphere2(glm::vec3(1.7f, .0f, 2.75f), 0.3f, maroon);

	Plane plane(glm::vec3(.0f, 1.0f, .0f), glm::vec3(.0f, -1.0f, .0f), white);

	// Generating Camera  
	glm::vec3 cameraPos(.0f, .35f, 0.0f);
	glm::vec3 cameraForward(.0f, .0f, 1.0f);
	glm::vec3 cameraReferUp(.0f, 1.0f, .0f);
	glm::vec3 cameraRight(1.0f, .0f, .0f);
	Camera cam(cameraPos, cameraForward, cameraReferUp);

	// Fill list of scene objects
	std::vector<Object*> sceneObjects;
	sceneObjects.push_back(&scene_sphere);
	sceneObjects.push_back(&scene_sphere2);

	sceneObjects.push_back(&plane);

	// Fill Lights into a collection
	std::vector<LightSources*> lights; 
	lights.push_back(&theLight);

	// world space to screen space
	float alpha, beta;
	glm::vec3 rayDir, rayOrigin;

	Color* tempColor = new Color[aaDepth * aaDepth];
	std::vector<double> tempR;
	std::vector<double> tempG;
	std::vector<double> tempB;

	int aaIdx = 0;

	for (int y = 0; y < options.height; y++) {
		for (int x = 0; x < options.width; x++) {
			Color finalColor;
			// Render w/o anti-aliasing
			if (aaDepth == 1) {
				alpha = ((2 * (x + 0.5) / (float)options.width) - 1.0f)
					* options.aspectRatio * tan(options.fov / 2);
				beta = (1 - (2 * (y + 0.5) / (float)options.height)) * tan(options.fov / 2);
 
				rayDir = normalize(glm::vec3(alpha, beta, .0f) + cam.getCamDir());
				rayOrigin = cameraPos;
				Ray camRay(rayOrigin, rayDir);

				std::vector<double> intersections;
				// store all intersection distances (even if they are negative, we'll weed
				// them out later in findClosestObjIndeX() function)
				for (int idx = 0; idx < sceneObjects.size(); idx++) {
					float d = sceneObjects[idx]->findIntersection(camRay);
					intersections.push_back(d);
				}

				// find the index of closest object, get its color
				int closestIndex = findClosestObjIndex(intersections);

				// negative index means no intersection, use default color and
				// move on to next pixel. 
				// 2nd condition ensures that the intersection value shld be atleast 
				// greater than the epsilon value T_MIN_VAL

				// else, save the pixel color of the closest object (accounting for
				// lights and shadows)
				if (closestIndex < 0 || intersections[closestIndex] < T_MIN_VAL) {
					continue;
				}
				else {
					Object* closestObj = sceneObjects[closestIndex];
					//Color surfaceColor = closestObj->getColor();
					// find intersection point
					glm::vec3 intersectPt =
						rayOrigin + (float)intersections[closestIndex] * rayDir;

					finalColor = getColorAt(intersectPt, rayDir, lights,
						sceneObjects, closestIndex, options.ambientLight, 0);
					finalColor = finalColor.colorClip();
				}
			}
			// Render with Anti-aliasing 
			else {
				// add another double for loop here for anti-aliasing
				for (int aay = 0; aay < aaDepth; aay++) {
					for (int aax = 0; aax < aaDepth; aax++) {
						aaIdx = aax + aay * aaDepth;
						alpha = ((2 * (x + (float)aax / ((float)aaDepth - 1) ) / (float)options.width) - 1.0f)
							* options.aspectRatio * tan(options.fov / 2);
						beta = (1 - (2 * (y + (float)aay / ((float)aaDepth - 1)) / (float)options.height)) * tan(options.fov / 2);

						rayDir = normalize(glm::vec3(alpha, beta, .0f) + cam.getCamDir());
						rayOrigin = cameraPos;
						Ray camRay(rayOrigin, rayDir);

						std::vector<double> intersections;
						// store all intersection distances (even if they are negative, we'll weed
						// them out later in findClosestObjIndeX() function)
						for (int idx = 0; idx < sceneObjects.size(); idx++) {
							float d = sceneObjects[idx]->findIntersection(camRay);
							intersections.push_back(d);
						}

						// find the index of closest object, get its color
						int closestIndex = findClosestObjIndex(intersections);

						// negative index means no intersection, use default color and
						// move on to next pixel. 
						// 2nd condition ensures that the intersection value shld be atleast 
						// greater than the epsilon value T_MIN_VAL

						// else, save the pixel color of the closest object (accounting for
						// lights and shadows)
						if (closestIndex < 0 || intersections[closestIndex] < T_MIN_VAL) {
							// crazy color issue: b/c you didn't assign Color object to this element
							// idx, instead you called setColorR/G/B on a non-existent object
							// at index aaIdx in the tempColor array
							tempColor[aaIdx] = Color(0.0f, 0.0f, 0.0f, 0.0f);
							continue;
						}
						else {
							Object* closestObj = sceneObjects[closestIndex];
							//Color surfaceColor = closestObj->getColor();
							// find intersection point
							glm::vec3 intersectPt =
								rayOrigin + (float)intersections[closestIndex] * rayDir;

							// getColorAt returns the clipped color
							Color tempCol = getColorAt(intersectPt, rayDir, lights,
								sceneObjects, closestIndex, options.ambientLight, 0);
							//tempR.push_back(tempCol.getColorR());
							//tempG.push_back(tempCol.getColorG());
							//tempB.push_back(tempCol.getColorB());
							tempColor[aaIdx] = tempCol;
						}
					}
				}

				// average all the R,G,B components
				double avgR = 0;
				double avgG = 0;
				double avgB = 0;
				for (int k = 0; k < aaDepth * aaDepth; k++) {
					//avgR += tempR[k];
					//avgG += tempG[k];
					//avgB += tempB[k];
					avgR += tempColor[k].getColorR();
					avgG += tempColor[k].getColorG();
					avgB += tempColor[k].getColorB();
				}
				finalColor.setColorR(avgR / ((double)aaDepth * aaDepth));
				finalColor.setColorG(avgG / ((double)aaDepth * aaDepth));
				finalColor.setColorB(avgB / ((double)aaDepth * aaDepth));
			}
			
			// apply the color to the final image
			colorBuffer[x + y * options.width].r = finalColor.getColorR();
			colorBuffer[x + y * options.width].g = finalColor.getColorG();
			colorBuffer[x + y * options.width].b = finalColor.getColorB();
			// TO-DO: cast generated rays into the scene 
			// using a pointer to a list of Objects, call intersection function
			// continue this url: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
		}
	}

	std::string fileName = "testFile.jpg";
	writeImage(fileName, 1.0, 2.2, colorBuffer, options.width, options.height);
	t2 = clock();
	std::cout << "Render time: " << (float)(t2 - t1) / 1000.0f << " seconds" << std::endl;
	// Free memory
	delete[] colorBuffer;
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