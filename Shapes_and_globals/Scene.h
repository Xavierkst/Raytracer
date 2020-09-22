#ifndef _SCENE_H_
#define _SCENE_H_
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Color.h"
#include "Sphere.h"
#include "Plane.h"
#include "Rect.h"
#include "Box.h"
#include "../Lights/Light.h"
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

// Curate some scenes with objects & lights
// (scene 1, 2, etc.) and load them in selectScene() function

// SCENE 1 -------------------------------------------------------

// Objects ------------------------------
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

// Bottom
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

// Boxes --------------------------------
extern Box box;
extern Box box2;
// Lights -------------------------------
extern Light theLight;
// area light
extern Light areaLight;

// SCENE 2 -------------------------------------------------------
// 4 boxes, 1 stacked on 3
// area light
extern glm::vec3 scene2_edge_a;
extern glm::vec3 scene2_edge_b;
extern glm::vec3 scene2_corner;
// 1 corner and 2 edges
extern  Rect scene2_rec;
// area light
extern Light scene2_areaLight;

extern glm::vec3 scene2_boxMid;
extern glm::vec3 scene2_boxMid2;
extern glm::vec3 scene2_boxMid3;
extern glm::vec3 scene2_boxMid4;

extern Box scene2_box1;
extern Box scene2_box2;
extern Box scene2_box3;
extern Box scene2_box4;
extern Box scene2_box5;

// Bottom
extern  Plane scene2_plane;
// Top
extern Plane scene2_plane2;
// L & R
extern Plane scene2_plane3;
extern  Plane scene2_plane4;
// Back wall
extern  Plane scene2_plane5;
// Wall behind camera
extern Plane scene2_plane6;

// Lights -------------------------------
extern Light scene2_theLight;
// area light
extern Light scene2_areaLight;

// SCENE 3 -------------------------------------------------------
// 1 opaque ball + box behind a transparent glass ball infront (w/ high ior)
// this is to show reflection n refraction fresnel effect
// use different color checkered floor 
// area light
extern glm::vec3 scene3_edge_a;
extern glm::vec3 scene3_edge_b;
extern glm::vec3 scene3_corner;
// 1 corner and 2 edges
extern  Rect scene3_rec;
// Bottom
extern  Plane scene3_plane1;
// Objects ------------------------------
// center
extern  Sphere scene3_sphere1;
extern  Sphere scene3_sphere2;
extern glm::vec3 scene3_boxMid;

extern Box scene3_box;

// Lights -------------------------------
extern Light scene3_theLight;
// area light
extern Light scene3_areaLight;

// SCENE 4 -------------------------------------------------------
// transparent and glass balls arranged in random order forming a curved arc
extern  Sphere scene4_sphere1;
extern  Sphere scene4_sphere2;
extern  Sphere scene4_sphere3;
extern  Sphere scene4_sphere4;
extern  Sphere scene4_sphere5;
extern  Sphere scene4_sphere6;
extern  Sphere scene4_sphere7;
extern  Sphere scene4_sphere8;
extern  Sphere scene4_sphere9;
extern  Sphere scene4_sphere10;
extern  Sphere scene4_sphere11;
extern  Sphere scene4_sphere12;
extern  Sphere scene4_sphere13;
extern  Sphere scene4_sphere14;
extern  Sphere scene4_sphere15;
extern  Sphere scene4_sphere16;

// Bottom
extern  Plane scene4_plane1;

// Lights -------------------------------
extern Light scene4_theLight;
// area light
extern Light scene4_areaLight;

#endif