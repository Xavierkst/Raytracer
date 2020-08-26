#ifndef _SCENE_OBJECTS_H_
#define _SCENE_OBJECTS_H_

#include <glm/glm.hpp>

#include "Color.h"
#include "Sphere.h"
#include "Plane.h"
#include "Rect.h"

#include "Light.h"
#include "LightSources.h"

namespace scene {
	// Colors ----------------------------------------------------------
	Color whiteLight(1.0f, 1.0f, 1.0f, 0.0f);
	// special value of flooring set to 2
	Color maroon(.5f, .25f, .25f, 0.5f);
	Color white(1.0f, 1.0f, 1.0f, .0f);
	Color floor_white(1.0f, 1.0f, 1.0f, 2.0f);
	Color green(0.5f, 1.0f, 0.5f, 0.5f);
	Color gray(.5f, .5f, .5f, .0f);
	Color black(.0f, .0f, .0f, .0f);
	Color perinwinkle(199.0f / 255.0f, 206.0f / 255.0f, 234.0f / 255.0f, .0f);
	Color pastel_pink(255 / 255.0f, 154 / 255.0f, 162 / 255.0f, .0f);
	Color pastel_blue(199.0f / 255.0f, 206.0f / 255.0f, 234.0f / 255.0f, .0f);
	Color mint(181 / 255.0f, 234 / 255.0f, 215 / 255.0f, .0f);

	// Objects ----------------------------------------------------------
	// center
	Sphere scene_sphere(glm::vec3(.0f, .0f, -3.0f), 1.0f,
		pastel_blue, 3.3f, REFLECTION_AND_REFRACTION);
	// right
	Sphere scene_sphere2(glm::vec3(1.7f, -.4f, -2.80f),
		0.6f, maroon, FLT_MAX, REFLECTION);
	// left
	Sphere scene_sphere3(glm::vec3(-1.7f, -.4f, -2.80f),
		0.6f, mint, 1.03f, REFLECTION_AND_REFRACTION);

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

	// area light
	glm::vec3 edge_a(-1.0f, .0f, .0f);
	glm::vec3 edge_b(0.f, .0f, -1.0f);
	glm::vec3 corner(.5f, 1.9f, -1.5f);
	// 1 corner and 2 edges
	Rect rec(glm::vec3(corner.x, corner.y + .1, corner.z),
		edge_a, edge_b, whiteLight, LIGHT);

	Plane plane(glm::vec3(.0f, 1.0f, .0f),
		glm::vec3(1.0f, -1.0f, .0f), floor_white, DIFFUSE_AND_GLOSSY);

	// Lights ----------------------------------------------------------
	Light theLight(glm::vec3(-7.0f, 5.0f, 3.0f),
		whiteLight, POINT_LIGHT,
		glm::vec3(.0f), glm::vec3(.0f), glm::vec3(.0f));

	// area light
	Light areaLight(corner, whiteLight,	AREA_LIGHT, corner,
		edge_a, edge_b);
}
#endif