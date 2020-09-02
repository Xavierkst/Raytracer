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
#include "LightSources.h"

namespace scene {
	// Colors ----------------------------------------------------------
	static Color whiteLight(1.0f, 1.0f, 1.0f, 0.0f);
	static Color red(221 / 255.0f, 119 / 255.0f, 119 / 255.0f, .0f);
	static Color blue(119 / 255.0f, 119 / 255.0f, 221 / 255.0f, 0.0f);
	static Color green(119 / 255.0f, 221 / 255.0f, 119 / 255.0f, 0.0f);

	// special value of flooring set to 2
	static Color maroon(.5f, .25f, .25f, 0.5f);
	static Color white(1.0f, 1.0f, 1.0f, .0f);
	static Color floor_white(1.0f, 1.0f, 1.0f, 2.0f);
	static Color grey(.7f, .7f, .7f, .0f);
	static Color black(.0f, .0f, .0f, .0f);
	static Color periwinkle(
		199.0f / 255.0f, 206.0f / 255.0f, 234.0f / 255.0f, .0f);
	static Color pastel_pink(
		255 / 255.0f, 154 / 255.0f, 162 / 255.0f, .0f);
	static Color pastel_blue(
		199.0f / 255.0f, 206.0f / 255.0f, 234.0f / 255.0f, .0f);
	static Color mint(
		181 / 255.0f, 234 / 255.0f, 215 / 255.0f, .0f);

	// Objects ----------------------------------------------------------
	// center
	static  Sphere scene_sphere(glm::vec3(.0f, .0f, -3.0f), 1.0f,
		pastel_blue, 3.6f, REFLECTION_AND_REFRACTION);
	// right
	static  Sphere scene_sphere2(glm::vec3(1.7f, -.4f, -2.80f),
		0.6f, maroon, FLT_MAX, REFLECTION);
	// left
	static  Sphere scene_sphere3(glm::vec3(-1.7f, -.4f, -2.80f),
		0.6f, mint, /*1.03f*/2.2f, REFLECTION_AND_REFRACTION);

	// b.g. mid-left
	static  Sphere scene_sphere4(glm::vec3(-.4f, -.65f, -5.3f), 0.35f,
		pastel_pink, FLT_MAX, DIFFUSE_AND_GLOSSY);
	// b.g. mid-right
	static  Sphere scene_sphere5(glm::vec3(.4f, -.65f, -5.3f), 0.35f,
		pastel_blue, FLT_MAX, DIFFUSE_AND_GLOSSY);
	// foreground left
	static  Sphere scene_sphere6(glm::vec3(-.5f, -.75f, -1.7f), 0.25f,
		pastel_blue, FLT_MAX, DIFFUSE_AND_GLOSSY);
	// b.g. left most
	static  Sphere scene_sphere7(glm::vec3(-3.9f, .0f, -5.3f), 0.35f,
		pastel_pink, FLT_MAX, DIFFUSE_AND_GLOSSY);

	// area light
	static glm::vec3 edge_a(-.5f, .0f, .0f);
	static glm::vec3 edge_b(0.f, .0f, -.5f);
	static glm::vec3 corner(.1f, 1.9f, -2.0f);
	// 1 corner and 2 edges
	static  Rect rec(glm::vec3(corner.x, corner.y + .01f, corner.z),
		edge_a, edge_b, whiteLight, LIGHT);

	//// Bottom
	static  Plane plane(glm::vec3(.0f, 1.0f, .0f),
		glm::vec3(1.0f, -1.0f, .0f), floor_white, DIFFUSE);
	// Top
	static Plane plane2(glm::vec3(.0f, -1.0f, .0f),
		glm::vec3(.0f, corner.y + .2f, .0f), grey, DIFFUSE);
	// L & R
	static Plane plane3(glm::vec3(1.0f, 0.0f, .0f),
		glm::vec3(-3.0f, 0.0f, .0f), red, DIFFUSE);
	static  Plane plane4(glm::vec3(-1.0f, 0.0f, .0f),
		glm::vec3(3.0f, 0.0f, .0f), blue, DIFFUSE);
	// Back wall
	static  Plane plane5(glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(.0f, 0.0f, -5.0f), grey, DIFFUSE);
	// Wall behind camera
	static Plane plane6(glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(.0f, 0.0f, .5f), grey, DIFFUSE);

	static glm::vec3 boxMid(1.2f, -.75f, -1.4f);
	static glm::vec3 boxMid2(-.4f, -.75f, -1.5f);

	// Boxes ----------------------------------------------------------
	static Box box(
		glm::vec3(boxMid.x - 0.25f, boxMid.y + .25f, boxMid.z - .25f),
		glm::vec3(boxMid.x + 0.25f, boxMid.y - 0.25f, boxMid.z + 0.25f), 
		pastel_pink, REFLECTION_AND_REFRACTION);

	static Box box2(
		glm::vec3(boxMid2.x - 0.25f, boxMid2.y + .25f, boxMid2.z - .25f),
		glm::vec3(boxMid2.x + 0.25f, boxMid2.y - 0.25f, boxMid2.z + 0.25f),
		mint, DIFFUSE);

	// Lights ----------------------------------------------------------
	static Light theLight(
		glm::vec3(-7.0f, 5.0f, 3.0f),
		whiteLight, POINT_LIGHT,
		glm::vec3(.0f), glm::vec3(.0f), glm::vec3(.0f));

	// area light
	static Light areaLight(corner, whiteLight,	AREA_LIGHT, corner,
		edge_a, edge_b);
}
#endif