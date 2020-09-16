#include "Scene.h"

 Color whiteLight(1.0f, 1.0f, 1.0f, 0.0f);
 Color red(221 / 255.0f, 119 / 255.0f, 119 / 255.0f, .0f);
 Color blue(119 / 255.0f, 119 / 255.0f, 221 / 255.0f, 0.0f);
 Color green(119 / 255.0f, 221 / 255.0f, 119 / 255.0f, 0.0f);

 Color maroon(.5f, .25f, .25f, 0.5f);
 Color white(1.0f, 1.0f, 1.0f, .0f);
 Color floor_white(1.0f, 1.0f, 1.0f, 2.0f);
 Color grey(.7f, .7f, .7f, .0f);
 Color black(.0f, .0f, .0f, .0f);
 Color periwinkle(
	199.0f / 255.0f, 206.0f / 255.0f, 234.0f / 255.0f, .0f);
Color pastel_pink(
	255 / 255.0f, 154 / 255.0f, 162 / 255.0f, .0f);
Color pastel_blue(
	199.0f / 255.0f, 206.0f / 255.0f, 234.0f / 255.0f, .0f);
Color mint(
	181 / 255.0f, 234 / 255.0f, 215 / 255.0f, .0f);

Sphere scene_sphere(glm::vec3(.0f, .0f, -3.0f), 1.0f,
	pastel_blue, 3.6f, REFLECTION_AND_REFRACTION);
// right
Sphere scene_sphere2(glm::vec3(1.7f, -.4f, -2.80f),
	0.6f, maroon, FLT_MAX, REFLECTION);
// left
Sphere scene_sphere3(glm::vec3(-1.7f, -.4f, -2.80f),
	0.6f, mint, /*1.03f*/2.2f, REFLECTION_AND_REFRACTION);

// b.g. mid-left
Sphere scene_sphere4(glm::vec3(-.4f, -.65f, -5.3f), 0.35f,
	pastel_pink, FLT_MAX, DIFFUSE_AND_GLOSSY);
// b.g. mid-right
Sphere scene_sphere5(glm::vec3(.4f, -.65f, -5.3f), 0.35f,
	pastel_blue, FLT_MAX, DIFFUSE_AND_GLOSSY);
// foreground left
Sphere scene_sphere6(glm::vec3(-.5f, -.75f, -1.7f), 0.25f,
	pastel_blue, FLT_MAX, DIFFUSE_AND_GLOSSY);
// b.g. left most
Sphere scene_sphere7(glm::vec3(-3.9f, .0f, -5.3f), 0.35f,
	pastel_pink, FLT_MAX, DIFFUSE_AND_GLOSSY);


glm::vec3 edge_a(-.5f, .0f, .0f);
glm::vec3 edge_b(0.f, .0f, -.5f);
glm::vec3 corner(.1f, 1.9f, -2.0f);
// 1 corner and 2 edges
Rect rec(glm::vec3(corner.x, corner.y + .01f, corner.z),
	edge_a, edge_b, whiteLight, LIGHT);

//// Bottom
Plane plane(glm::vec3(.0f, 1.0f, .0f),
	glm::vec3(1.0f, -1.0f, .0f), floor_white, DIFFUSE);
// Top
Plane plane2(glm::vec3(.0f, -1.0f, .0f),
	glm::vec3(.0f, corner.y + .2f, .0f), grey, DIFFUSE);
// L & R
Plane plane3(glm::vec3(1.0f, 0.0f, .0f),
	glm::vec3(-3.0f, 0.0f, .0f), red, DIFFUSE);
Plane plane4(glm::vec3(-1.0f, 0.0f, .0f),
	glm::vec3(3.0f, 0.0f, .0f), blue, DIFFUSE);
// Back wall
Plane plane5(glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(.0f, 0.0f, -5.0f), grey, DIFFUSE);
// Wall behind camera
Plane plane6(glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(.0f, 0.0f, .5f), grey, DIFFUSE); 

glm::vec3 boxMid(1.2f, -.75f, -1.4f);
glm::vec3 boxMid2(-.4f, -.75f, -1.5f);

// Boxes ----------------------------------------------------------
//Box box(
//	glm::vec3(boxMid.x - 0.25f, boxMid.y + .25f, boxMid.z + .25f),
//	glm::vec3(boxMid.x + 0.25f, boxMid.y - 0.25f, boxMid.z - 0.25f),
//	pastel_pink, REFLECTION_AND_REFRACTION);
Box box(boxMid, 0.5f, pastel_pink, 1.09f, REFLECTION_AND_REFRACTION);

//Box box2(
//	glm::vec3(boxMid2.x + 0.25f, boxMid2.y - 0.25f, boxMid2.z - 0.25f),
//	glm::vec3(boxMid2.x - 0.25f, boxMid2.y + .25f, boxMid2.z + .25f),
//	mint, DIFFUSE
Box box2(boxMid2, 0.5f, mint, 1.05f, DIFFUSE);

// Lights ----------------------------------------------------------
Light theLight(
	glm::vec3(-7.0f, 5.0f, 3.0f),
	whiteLight, POINT_LIGHT,
	glm::vec3(.0f), glm::vec3(.0f), glm::vec3(.0f));

// area light
Light areaLight(corner, whiteLight, AREA_LIGHT, corner,
	edge_a, edge_b);
