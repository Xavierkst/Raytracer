#include "time.h"
#include "Render.h"
#define _CRTDBG_MAP_ALLOC //to get more details
#include <stdlib.h>  
#include <crtdbg.h>   //for malloc and free

using namespace scene;


int main(int argc, char* argv[]) {
	// checking for memory leaks
	_CrtMemState sOld;
	_CrtMemState sNew;
	_CrtMemState sDiff;
	_CrtMemCheckpoint(&sOld); //take a snapshot

	std::cout << "Rendering... " << std::endl;
	// Rendering image options (fov, width, height etc.)
	Options options; 

	// Record rendering time elapsed
	clock_t t1, t2; 
	t1 = clock();

	// initializing all pixels in frame buffer to default value 
	Color* colorBuffer = new Color[options.width*options.height];
	for (int i = 0; i < options.width*options.height; i++) {
		colorBuffer[i] = options.backgroundColor;
	}
	// Colors / object types / lights found in "Scene.h"

	// Populate scene objects -----------------------------------------------------
	std::vector< Object*> sceneObjects;
	sceneObjects.push_back(&scene_sphere);
	sceneObjects.push_back(&scene_sphere2);
	sceneObjects.push_back(&scene_sphere3);
	sceneObjects.push_back(&scene_sphere4);
	sceneObjects.push_back(&scene_sphere5);
	sceneObjects.push_back(&scene_sphere6);
	sceneObjects.push_back(&scene_sphere7);

	sceneObjects.push_back(&plane);
	sceneObjects.push_back(&plane2);
	sceneObjects.push_back(&plane3);
	sceneObjects.push_back(&plane4);
	sceneObjects.push_back(&plane5);
	sceneObjects.push_back(&plane6);

	sceneObjects.push_back(&rec);
	sceneObjects.push_back(&box);
	//sceneObjects.push_back(&box2);
	
	// Populate Lights  ---------------------------------------------------------
	std::vector< LightSources*> lights;
	//lights.push_back(&theLight);
	lights.push_back(&areaLight);

	// Generating Camera --------------------------------------------------------
	glm::vec3 cameraPos(.0f, -.2f, 0.0f);
	glm::vec3 cameraForward(.0f, .0f, -1.0f);
	glm::vec3 cameraReferUp(.0f, 1.0f, .0f);
	glm::vec3 cameraRight(1.0f, .0f, .0f);
	Camera cam(cameraPos, cameraForward, cameraReferUp);

	// generate 2 size n^2 arrays holding randomized values in range [0, 1)
	glm::vec2* r = new glm::vec2[options.sampleNum * options.sampleNum];
	glm::vec2* s = new glm::vec2[options.sampleNum * options.sampleNum];

	Render renderer;
	// Begin rendering ---------------------------------------------------------
	renderer.render(lights, sceneObjects, colorBuffer, cam, options, r, s);

	std::string fileName = "testFile.jpg";
	renderer.writeImage(fileName, 1.0, 2.2, colorBuffer, 
		options.width, options.height);
	// Stop recording time ------------------------------------------------------
	t2 = clock();
	std::cout << "Render time: " << 
		(float)(t2 - t1) / 1000.0f << " seconds" << std::endl;

	// Free memory ----------------------------------------------------------
	delete[] colorBuffer;
	delete[] r; 
	delete[] s;

	while (!sceneObjects.empty()) {
		sceneObjects.pop_back();
	}
	sceneObjects.clear();

	// Memory Leak Summary
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