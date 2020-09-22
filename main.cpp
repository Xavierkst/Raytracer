#include "time.h"

#define _CRTDBG_MAP_ALLOC //to get more details
#include <stdlib.h>  
#include <crtdbg.h>   //for malloc and free
#include "Render.h"
#include "Shapes_and_globals/Scene.h"

//void writeImage(std::string fileName, float exposure,
//	float gamma, Color* pixelData, int width, int height);

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

	// Generating Camera --------------------------------------------------------
	Camera cam(
		options.cameraPos, 
		options.cameraForward, 
		options.cameraReferUp);

	// generate 2 size n^2 arrays holding randomized values in range [0, 1)
	// r -- cam ray x-y jitter values
	// s -- shadow ray x-y jitter values
	glm::vec2* r = new glm::vec2[options.sampleNum * options.sampleNum];
	glm::vec2* s = new glm::vec2[options.sampleNum * options.sampleNum];

	// Populate scene objects & Lights ------------------------------------------
	Render renderer;
	
	// Objects
	std::vector< Object*> sceneObjects;
	std::vector< LightSources*> lights;

	renderer.selectScene(sceneObjects, lights, options.selectScene);
	// BEGIN RENDERING ---------------------------------------------------------

	renderer.startRender(lights, 
		sceneObjects, colorBuffer, cam, options, r, s);
	
	std::string outFileName = "rendered_images/testFile.jpg";
	renderer.writeImage(outFileName, 
		1.0f, 2.2f, colorBuffer, options.width, options.height);

	// Stop recording time ------------------------------------------------------
	t2 = clock();
	std::cout << "Render time: " << 
		(float)(t2 - t1) / 1000.0f << " seconds" << std::endl;

	// Free memory --------------------------------------------------------------
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