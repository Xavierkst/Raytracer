#ifndef _SCENE_H_
#define _SCENE_H_
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Color.h"
#include "Sphere.h"
#include "Plane.h"
#include "Rect.h"
#include "Box.h"
#include "Light.h"
#include <vector>

	// Colors --------------------------------------------------------
	extern Color whiteLight;
	extern Color red;
	extern Color blue;
	extern Color green;

	// special value of flooring set to 2
	extern Color maroon;
	extern Color white;
	extern Color floor_white;
	extern Color grey;
	extern Color black;
	extern Color periwinkle;
	extern Color pastel_pink;
	extern Color pastel_blue;
	extern Color mint;

	// Objects -------------------------------------------------------
	// center
	extern  Sphere scene_sphere;
	// right
	extern  Sphere scene_sphere2;
	// left
	extern  Sphere scene_sphere3;

	// b.g. mid-left
	extern  Sphere scene_sphere4;
	// b.g. mid-right
	extern  Sphere scene_sphere5;
	// foreground left
	extern  Sphere scene_sphere6;
	// b.g. left most
	extern  Sphere scene_sphere7;

	// area light
	extern glm::vec3 edge_a;
	extern glm::vec3 edge_b;
	extern glm::vec3 corner;
	// 1 corner and 2 edges
	extern  Rect rec;

	//// Bottom
	extern  Plane plane;
	// Top
	extern Plane plane2;
	// L & R
	extern Plane plane3;
	extern  Plane plane4;
	// Back wall
	extern  Plane plane5;
	// Wall behind camera
	extern Plane plane6;
	extern glm::vec3 boxMid;
	extern glm::vec3 boxMid2;

	// Boxes ----------------------------------------------------------
	extern Box box;
	extern Box box2;
	// Lights ----------------------------------------------------------
	extern Light theLight;
	// area light
	extern Light areaLight;

	// Curate some scenes with objects & lights
	// (scene 1, 2, etc.) by loading them manually

	// function to choose which particular scene to load
	//void loadScene(int sceneNum);

#endif