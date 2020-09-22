#include "Scene.h"

// COLOR PALLETE -------------------------------------------------
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
Color pastel_yellow(253 / 255.0f, 253 / 255.0f, 151/255.0f, .0f);

// SCENE 1 -------------------------------------------------------
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


// Boxes ----------------------------------------------------------
glm::vec3 boxMid(1.2f, -.75f, -1.4f);
glm::vec3 boxMid2(-.4f, -.75f, -1.5f);
Box box(boxMid, 0.5f, pastel_pink, 1.09f, REFLECTION_AND_REFRACTION);
Box box2(boxMid2, 0.5f, mint, 1.05f, DIFFUSE);

// Lights ----------------------------------------------------------
// Rect geom for lights
glm::vec3 edge_a(-.5f, .0f, .0f);
glm::vec3 edge_b(0.f, .0f, -.5f);
glm::vec3 corner(.1f, 7.9f, -2.0f);
// 1 corner and 2 edges
Rect rec(glm::vec3(corner.x, corner.y + .01f, corner.z),
	edge_a, edge_b, whiteLight, LIGHT);

// area light
Light areaLight(corner, whiteLight, AREA_LIGHT, corner,
	edge_a, edge_b);

Light theLight(
	glm::vec3(-7.0f, 5.0f, 3.0f),
	whiteLight, POINT_LIGHT,
	glm::vec3(.0f), glm::vec3(.0f), glm::vec3(.0f));

// SCENE 2 -------------------------------------------------------
// 4 boxes, 1 stacked on 3
glm::vec3 scene2_edge_a(-.6f, .0f, .0f);
glm::vec3 scene2_edge_b(0.f, .0f, -.6f);
glm::vec3 scene2_corner/*(-.4f, 1.9f, -2.0f)*/(-.6f, 3.9f, -2.0f);

Rect scene2_rec(glm::vec3(scene2_corner.x, scene2_corner.y + .01f, scene2_corner.z),
	scene2_edge_a, scene2_edge_b, whiteLight, LIGHT);

glm::vec3 scene2_boxMid(0.0f, -.75f, -1.4f);
glm::vec3 scene2_boxMid2(0.5f, -.75f, -1.4f);
glm::vec3 scene2_boxMid3(-0.5f, -.75f, -1.4f);
glm::vec3 scene2_boxMid4(-.3f, -0.25f, -1.6f);
glm::vec3 scene2_boxMid5(0.5f, -0.05f, -2.6f);

// pos, edge length, color, ior, material type
Box scene2_box1(scene2_boxMid, 0.5f, blue, 1.09f, DIFFUSE_AND_GLOSSY);
Box scene2_box2(scene2_boxMid2, 0.5f, pastel_pink, 1.09f, REFLECTION_AND_REFRACTION);
Box scene2_box3(scene2_boxMid3, 0.5f, pastel_blue, 1.09f, DIFFUSE_AND_GLOSSY);
Box scene2_box4(scene2_boxMid4, 0.3f, mint, 1.29f, REFLECTION_AND_REFRACTION);
Box scene2_box5(scene2_boxMid5, 0.6f, pastel_pink, 1.09f, DIFFUSE_AND_GLOSSY);

//// Bottom
Plane scene2_plane(glm::vec3(.0f, 1.0f, .0f),
	glm::vec3(1.0f, -1.0f, .0f), floor_white, DIFFUSE);
// Top
Plane scene2_plane2(glm::vec3(.0f, -1.0f, .0f),
	glm::vec3(.0f, corner.y + .2f, .0f), floor_white, DIFFUSE);
// L & R
Plane scene2_plane3(glm::vec3(1.0f, 0.0f, .0f),
	glm::vec3(-3.0f, 0.0f, .0f), red, DIFFUSE);
Plane scene2_plane4(glm::vec3(-1.0f, 0.0f, .0f),
	glm::vec3(3.0f, 0.0f, .0f), blue, DIFFUSE);
// Back wall
Plane scene2_plane5(glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(.0f, 0.0f, -5.0f), grey, DIFFUSE);
// Wall behind camera
Plane scene2_plane6(glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(.0f, 0.0f, .5f), grey, DIFFUSE);

// area light
Light scene2_areaLight(scene2_corner, whiteLight, AREA_LIGHT, scene2_corner,
	scene2_edge_a, scene2_edge_b);

Light scene2_theLight(
	glm::vec3(-7.0f, 5.0f, 3.0f),
	whiteLight, POINT_LIGHT,
	glm::vec3(.0f), glm::vec3(.0f), glm::vec3(.0f));

// SCENE 3 -------------------------------------------------------
// 1 opaque ball + box behind a transparent glass ball infront (w/ high ior)
// this is to show reflection n refraction fresnel effect
// use different color checkered floor
glm::vec3 scene3_edge_a(-.5f, .0f, .0f);
glm::vec3 scene3_edge_b(0.f, .0f, -.5f);
glm::vec3 scene3_corner(.1f, 1.9f, -2.0f);

Rect scene3_rec(glm::vec3(scene3_corner.x, scene3_corner.y + .01f, scene3_corner.z),
	scene3_edge_a, scene3_edge_b, whiteLight, LIGHT);

// Bottom
Plane scene3_plane1(glm::vec3(.0f, 1.0f, .0f),
	glm::vec3(1.0f, -1.0f, .0f), floor_white, DIFFUSE);
// Objects --------------
// center
Sphere scene3_sphere1(glm::vec3(.0f, .0f, -2.5f), 1.0f,
	white, 1.035f, REFLECTION_AND_REFRACTION);
Sphere scene3_sphere2(glm::vec3(-1.0f, .0f, -4.4f), .7f,
	pastel_yellow, 3.6f, DIFFUSE_AND_GLOSSY);

// pos, edge length, color, ior, material type
Box scene3_box(glm::vec3(.7f, .0f, -4.4f), 0.8f, pastel_pink, 1.22f, DIFFUSE_AND_GLOSSY);;

// area light
Light scene3_areaLight(scene3_corner, whiteLight, AREA_LIGHT, scene3_corner,
	scene3_edge_a, scene3_edge_b);

Light scene3_theLight(
	glm::vec3(-7.0f, 5.0f, 3.0f),
	whiteLight, POINT_LIGHT,
	glm::vec3(.0f), glm::vec3(.0f), glm::vec3(.0f));

// SCENE 4 -------------------------------------------------------
// transparent and glass balls arranged in random order forming a curved arc
// transparent and glass balls arranged in random order forming a curved arc
Sphere scene4_sphere1(glm::vec3(-2.3f, -.5f,  -1.0f), .5f, pastel_blue, 3.6f, REFLECTION);
Sphere scene4_sphere2(glm::vec3(-1.8f, -.5f, - .0f), .5f,	white, 1.025f, REFLECTION_AND_REFRACTION);
Sphere scene4_sphere3(glm::vec3(-1.2f, -.5f, -1.5f), .5f,	pastel_blue, 3.6f, REFLECTION);
Sphere scene4_sphere4(glm::vec3(- .5f, -.5f, -3.0f),.5f, white, 1.025f, REFLECTION_AND_REFRACTION);
Sphere scene4_sphere5(glm::vec3( 0.3f, -.5f, -4.5f), .5f, pastel_blue, 3.6f, REFLECTION);
Sphere scene4_sphere6(glm::vec3( 1.3f, -.5f, -6.0f), .5f, white, 1.025f, REFLECTION_AND_REFRACTION);
Sphere scene4_sphere7(glm::vec3( 2.5f, -.5f, -7.5f), .5f,	pastel_blue, 3.6f, REFLECTION);
Sphere scene4_sphere8(glm::vec3( 3.9f, -.5f, -9.0f), .5f, white, 1.025f, REFLECTION_AND_REFRACTION);
Sphere scene4_sphere9(glm::vec3( 5.5f, -.5f, -10.5f), .5f, pastel_blue, 3.6f, REFLECTION);
Sphere scene4_sphere10(glm::vec3(7.3f, -.5f, -12.0f), .5f, white, 1.025f, REFLECTION_AND_REFRACTION);
Sphere scene4_sphere11(glm::vec3(9.3f, -.5f, -13.5f), .5f, pastel_blue, 3.6f, REFLECTION);
Sphere scene4_sphere12(glm::vec3(11.5f, -.5f, -15.0f), .5f, white, 1.025f, REFLECTION_AND_REFRACTION);
Sphere scene4_sphere13(glm::vec3(13.9f, -.5f, -16.5f), .5f, pastel_blue, 3.6f, REFLECTION);
Sphere scene4_sphere14(glm::vec3(16.5f, -.5f, -18.0f), .5f, white, 1.025f, REFLECTION_AND_REFRACTION);
Sphere scene4_sphere15(glm::vec3(19.3f, -.5f, -19.5f), .5f, pastel_blue, 3.6f, REFLECTION);
Sphere scene4_sphere16(glm::vec3(8.7f, -.5f, -21.0f), .5f, white, 1.025f, REFLECTION_AND_REFRACTION);
Sphere scene4_sphere17(glm::vec3(8.7f, -.5f, -22.5f), .5f, pastel_blue, 3.6f, REFLECTION);
Sphere scene4_sphere18(glm::vec3(8.7f, -.5f, -24.0f), .5f, white, 1.025f, REFLECTION_AND_REFRACTION);
Sphere scene4_sphere19(glm::vec3(8.7f, -.5f, -25.5f), .5f, pastel_blue, 3.6f, REFLECTION);
Sphere scene4_sphere20(glm::vec3(8.7f, -.5f, -27.0f), .5f, white, 1.025f, REFLECTION_AND_REFRACTION);

// Bottom
Plane scene4_plane1(glm::vec3(.0f, 1.0f, .0f),
	glm::vec3(1.0f, -1.0f, .0f), floor_white, DIFFUSE);

glm::vec3 scene4_edge_a(-.5f, .0f, .0f);
glm::vec3 scene4_edge_b(0.f, .0f, -.5f);
glm::vec3 scene4_corner(.1f, 1.9f, -2.0f);

Rect scene4_rec(glm::vec3(scene4_corner.x, scene4_corner.y + .01f, scene4_corner.z),
	scene4_edge_a, scene4_edge_b, whiteLight, LIGHT);

// area light
Light scene4_areaLight(scene4_corner, whiteLight, AREA_LIGHT, scene4_corner,
	scene4_edge_a, scene4_edge_b);

Light scene4_theLight(
	glm::vec3(-7.0f, 5.0f, 3.0f),
	whiteLight, POINT_LIGHT,
	glm::vec3(.0f), glm::vec3(.0f), glm::vec3(.0f));