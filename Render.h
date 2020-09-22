#ifndef _RENDER_H_
#define _RENDER_H_
#include <iostream>
#include <string>
#include <vector>
#include "write_image_lib/utils.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Options.h"
#include "windows.h"
#include "Shapes_and_globals/Object.h"
#include "Lights/LightSources.h"
#include "Grid_Acceleration_Structure/Grid.h"
#include "Shapes_and_globals/Scene.h"

class Render {

private:
	//std::vector<Objects*> sceneObjects;

public:
	Render();
	Grid* sceneGrid;

	// The actual rendering function that generates camera and
	// camera rays to cast into each pixel
	void startRender(std::vector<LightSources*>& lights,
		std::vector<Object*>& sceneObjects,
		Color* colorBuffer, Camera cam,
		Options options, glm::vec2* r,
		glm::vec2* s);

	// sets the color data at the i,j-th pixel to be of color value
	// r, g, b
	void setPixelColor(int i, int j, Color* buffer, int width, 
		double r, double g, double b);

	// Given a ray position & dir, casts the ray into the scene
	// intersecting with scene objects (Sometimes recursively) and 
	// evaluating and returning the final color onto each pixel
	Color castRay(const glm::vec3& orig, const glm::vec3& dir,
		const std::vector<LightSources*>& sources,
		const std::vector<Object*>& objects,
		const Options opts,
		uint32_t depth,
		glm::vec2 jitter);

	// Shuffles the randomized float values within the 
	// populated array pointed tp by "s"
	void shuffleFloatArray(glm::vec2* s, int sampleNum);

	// Given a ray, computes ray intersections with all of the 
	// objects in the scene and
	// Stores intersection info of closest obj intersected.
	// returns true if object intersected
	// stores: tNear -- distance to nearest hitpoint
	// objIndex -- index of nearest object
	// uv -- 
	// hitObj -- stores pointer to the closest object encountered
	bool trace(glm::vec3 orig, glm::vec3 dir,
		const std::vector<Object*>& objects,
		float& tNear, int& objIndex,
		glm::vec2& uv, Object** hitObject);

	// returns a float value which is the value "v" if it is within
	// lo and hi, if not, if v is greater than hi, returns hi,
	// else if v is less than lo, returns lo
	float clamp(const float& lo, const float& hi, const float& v);

	// Computes the ratio of light reflected from transparent
	// object and store it into kr [0,1]
	// reflected (Kr)
	// I -- incident ray dir 
	// N -- surface normal
	// ior -- index of refraction
	// kr -- ratio of reflected light, whereas ratio of refracted is 1 - Kr
	void fresnel(const glm::vec3& I, glm::vec3& N, float& ior, float& kr);

	// Computes the refracted ray given Incident ray, normal, 
	// and idx of refraction
	glm::vec3 refract(glm::vec3 I, glm::vec3 N, float ior);

	// Generates and stores 2 vectors: Reflection ray origin 
	// (hit point accounting for bias) and reflection ray direction
	void reflect(glm::vec3 ray_dir, glm::vec3 N, glm::vec3 hitPoint,
		glm::vec3& reflect_orig, glm::vec3& reflect_dir, Options opts);

	// Executes the phong shading routine: 
	// accounts for: ambient, diffuse, and specular lighting 
	// returns surface Color after summing up all contributions
	Color phongShading(const glm::vec3 dir, const glm::vec3 N,
		const glm::vec3 hitPoint, Object* hitObj,
		const std::vector<LightSources*>& sources,
		const std::vector<Object*>& objects,
		const Options& opts, glm::vec2& jitter);

	// Writes the colorBuffer data into a vector of unsigned chars
	// then passed into resultToPNG function to be encoded
	void writeImage(std::string fileName, float exposure,
		float gamma, Color* pixelData, int width, int height);

	// Pass in an empty light and objects vector, 
	// as well as an integer value to choose a scene.
	// Fills vector w objects & lights for a specified scene. 
	// Objects/shapes and lights taken from "scene.h" file
	void selectScene(std::vector<Object*>& sceneObjects, 
		std::vector<LightSources*>& lightSources,
		int sceneNumber);
};
#endif