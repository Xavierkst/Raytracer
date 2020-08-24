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
#include "Options.h"
#include "time.h"

#include "windows.h"
#define _CRTDBG_MAP_ALLOC //to get more details
#include <stdlib.h>  
#include <crtdbg.h>   //for malloc and free


bool trace(glm::vec3 orig, glm::vec3 dir,
	const std::vector<Object*>& objects,
	float& tNear, int& objIndex,
	glm::vec2& uv, Object** hitObject);

float clamp(const float& lo, const float& hi, const float& v);

void writeImage(std::string fileName, float exposure, float gamma, Color* pixelData, int width, int height) {
	std::vector<unsigned char> imageData(width * height * 4);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Color pixelColor = pixelData[x + y * width];
			/*pixelColor.gammaCorrection(exposure, gamma);
			pixelColor.clamp();*/

			int index = 4 * (x + y * width);
			imageData[index + 0] = (unsigned char)(pixelColor.getColorR() * 255.0f);
			imageData[index + 1] = (unsigned char)(pixelColor.getColorG() * 255.0f);
			imageData[index + 2] = (unsigned char)(pixelColor.getColorB() * 255.0f);
			imageData[index + 3] = 255; // alpha channel
		}
	}
	std::cout << imageData.size() << std::endl;
	std::cout << height << " " << width << std::endl;
	resultToPNG(fileName, width, height, imageData);
}

// Computes the ratio of light reflected from transparent
// object and store it into kr [0,1]
// reflected (Kr)
// incidentDir -- incident ray dir 
// normal -- surface normal
// ior -- index of refraction
// kr -- ratio of reflected light, whereas ratio of refracted is 1 - Kr
void fresnel(const glm::vec3& I, glm::vec3& N, float& ior, float& kr) {
	// get the refraction index and eta 
	float etai = 1.0f; float etat = ior; 
	float cosi = clamp(-1, 1, dot(I, N));
	
	// check if ray is incoming from inside or outside the object
	// dot(I, N) >= 0 means coming from inside, so we swap the 
	// ref. indices, also flip the normal
	if (cosi > 0.0f) {
		std::swap(etai, etat); 
		//nRef = -N; // no need to swap normal
	}
	float eta = etai / etat;
	// check for total internal reflection, 
	// i.e. if 1 - sin(theta2)^2 term is negative
	// (or) sin(theta2) > 1, then its TIR
	float sint = eta * sqrtf(max(.0f, ( 1.0f -  cosi * cosi )));
	// either the critical angle is surpassed (sint>=1.0f) or 
	// obj's refractive index is infinity, i.e. obj is opaque. c/v = Infinity
	if (sint >= 1.0f || etat == FLT_MAX) {
		// TIR occurs 
		kr = 1.0f; 
	} 
	// if NOT TIR, theres some portion of 
	// incident light that gets transmitted
	else {
		float cost = sqrtf(max(0.0f, sqrtf(1 - sint*sint)));
		cosi = fabsf(cosi);
		float Rs = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		float Rp = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));

		kr = 0.5f * ((float)Rp * Rp + (float) Rs * Rs); 
	}
}


// Computes the refracted ray given Incident ray, normal, 
// and idx of refraction
 glm::vec3 refract(glm::vec3 I, glm::vec3 N, float ior) {
	// we assume incoming ray by default comes from the medium
	// of lower refractive index (i.e. from air to something denser)
	float cosi = clamp(-1, 1, dot(I, N));
	float etai = 1.0f; float etat = ior;
	glm::vec3 nRef = N;
	// test if ray is coming from medium of lower/higher ref idx
	if ( cosi < 0 ) {
		// ray coming from medium of lower ref idx,
		// cosi or cos(theta 1) will be negative, so negate that
		cosi = - cosi;
	}
	else {
		// coming from denser to less dense medium. Swap refract idxes
		std::swap(etai, etat);
		// negate the normal as well. 
		// dw about cosi, its unchanged since >= 0 alrdy
		nRef = -N;
	}

	float eta = etai / etat;
	// check for total internal reflection TIR -- see if  1- sin(theta2)^2
	// +ve or -ve, if negative, no portion of ray is transmitted
	float k = 1 - eta*eta*(1 - cosi * cosi);
	return k < 0.0f ? glm::vec3(.0f) : eta * I + nRef * (eta * cosi - sqrtf(k));
}
				

// Given a ray position & dir, casts the ray into the scene
// intersecting with scene objects (Sometimes recursively) and 
// evaluating and returning the final color onto each pixel
Color castRay(const glm::vec3& orig, const glm::vec3& dir,
	const std::vector<LightSources*>& sources,
	const std::vector<Object*>& objects,
	const Options opts,
	uint32_t depth,
	glm::vec2 jitter) {
	Color hitColor = Color();
	// check whether depth is greater than maxDepth, assign 
	// black background if so
	if (depth > MAX_RECURSION_DEPTH) {
		return hitColor = Color(.0f, .0f, .0f, .0f);
	}

	int objIndex; // stores idx of closest obj in scene list
	glm::vec2 uv; // check this 
	Object* hitObj = nullptr;
	float tNear = FLT_MAX;
	
	// find the closest object intersected 
	if (trace(orig, dir, objects, tNear, objIndex, uv, &hitObj)) {
		glm::vec3 hitPoint = orig + dir * tNear;
		glm::vec3 N; // normal
		glm::vec3 tmp = hitPoint;
		glm::vec2 st; // for triangle meshes

			// set floor tiles to be checkered
		if (hitObj->getColor().getColorSpecial() == 2.0f) {
			int squareTile = floor(hitPoint.x) + floor(hitPoint.z);
			if (squareTile % 2 == 0) {
				hitObj->setColor(0.0f, 0.0f, 0.0f);
			}
			else {
				hitObj->setColor(1.0f, 1.0f, 1.0f);
			}
		}

		// getSurfaceProperties returns normal of the surface only (for now)
		hitObj->getSurfaceProperties(hitPoint, dir, objIndex, uv, N, st);

		switch (hitObj->getMaterialType()) {
			case REFLECTION_AND_REFRACTION: {
				float kr = 0.0f; // ratio of reflected light
				float kt;
				// When primary ray incident on transparent material, 
				// 2 rays produced: Reflection and refraction ray

				// Generate refraction ray:
				// Adding bias: test if incoming ray from inside or outside obj
				glm::vec3 refract_ray_orig = dot(dir, N) < 0.0f ? 
					hitPoint - N * opts.bias : hitPoint + N * opts.bias;
				glm::vec3 refract_ray_dir = normalize(refract(dir, N, hitObj->ior));
				// compute fresnel 
				fresnel(dir, N, hitObj->ior, kr);
				// Proportion of light transmitted 
				kt = 1.0f - kr;
				// if amt of light reflected is less than 100% 
				if (kr < 1.0f) {
					// cast refraction ray:
					hitColor = hitColor + hitObj->getColor() * castRay(refract_ray_orig,
						refract_ray_dir, sources, objects, opts, ++depth, jitter) * kt;
				}
				//std::cout << kr << std::endl;
				
				// Generate reflection ray: 
				glm::vec3 reflection_dir = normalize(dir - 2.0f * dot(dir, N) * N);
				glm::vec3 reflection_ray_origin = (dot(reflection_dir, N) < 0.0f) ?
					hitPoint - N * opts.bias :
					hitPoint + N * opts.bias;
				hitColor = hitColor + castRay(reflection_ray_origin, reflection_dir, sources, objects, opts, ++depth, jitter) * kr;

				break;
			}
			case REFLECTION: { // object is perfectly a mirror
				float kr = .0f;

				// fresnel -- sets the normal, and sets Kr (reflect ratio)
				fresnel(dir, N, hitObj->ior, kr);

				// computer reflection direction
				glm::vec3 reflection_dir = normalize(dir - 2.0f * dot(dir, N) * N);
				// reflection_ray_origin biased to avoid shadow acne
				// if dot(R, N) < 0, ref ray inside the surface, else its outside surface
				glm::vec3 reflection_ray_origin = (dot(reflection_dir, N) < 0.0f) ? 
					hitPoint - N*opts.bias : 
					hitPoint + N*opts.bias;

				// make recursive call to castRay function to sample the color of 
				// the reflected ray cast out from the hitPoint 
				hitColor = hitColor + castRay(reflection_ray_origin, reflection_dir, sources, objects, opts, ++depth, jitter) * kr;
				break;	
			}
			case DIFFUSE_AND_GLOSSY_AND_REFLECTION: {
				// REFLECTIVE PART -- apply recursion
				float kr = .0f;
				// fresnel -- sets the normal, and sets Kr (reflect ratio)
				fresnel(dir, N, hitObj->ior, kr);

				// computer reflection direction
				glm::vec3 reflection_dir = normalize(dir - 2.0f * dot(dir, N) * N);
				// reflection_ray_origin biased to avoid shadow acne
				// if dot(R, N) < 0, ref ray inside the surface, else its outside surface
				glm::vec3 reflection_ray_origin = (dot(reflection_dir, N) < 0.0f) ?
					hitPoint - N * opts.bias :
					hitPoint + N * opts.bias;
				// make recursive call to castRay function to sample the color of 
				// the reflected ray cast out from the hitPoint 
				hitColor = hitColor + castRay(reflection_ray_origin, reflection_dir, sources, objects, opts, ++depth, jitter) * kr;

				// DIFFUSE & GLOSSY part, apply Blinn-Phong
				// add ambient color component
				hitColor = hitColor + hitObj->getColor() * opts.ambientLight;
				// iterate thru each light source and sum their contribution
				// kd (diffuse color) * I (light intensity) * dot(N, l) +
				// Ks * I * pow(dot(h, N), phongExponent);
				Color sumDiffuse = Color(); // initialized to black
				Color sumSpecular = Color();
				glm::vec3 shadowOrigPoint = (dot(dir, N) < 0) ?
					hitPoint + N * opts.bias :
					hitPoint - N * opts.bias;
				for (int i = 0; i < sources.size(); i++) {
					float tShadowNear = FLT_MAX;
					Object* shadowObj = nullptr;
					// get light direction,  
					glm::vec3 light_dir = sources[i]->getLightPos() - hitPoint;
					// squared distance from hit point to light source
					float light_distance_sq = dot(light_dir, light_dir);
					light_dir = normalize(light_dir);

					// trace rays back to lightsource and do intersection tests:
					// If an object intersected by shadow ray, and the object's is closer
					// to the shadowOrigin than the light, the region will be in shadow
					bool inShadow = trace(shadowOrigPoint, light_dir, objects,
						tShadowNear, objIndex, uv, &shadowObj) && (tShadowNear * tShadowNear) < light_distance_sq;


					// calculate diffuse contribution
					// not sure why you need to include the surface color in this equation
					// (1- inShadow) checks if i-th light being blocked by an object
					sumDiffuse = sumDiffuse + sources[i]->getLightColor() *
						max(0.0f, dot(N, light_dir)) * ((double)1 - inShadow);

					// calculate specular contribution
					glm::vec3 scalar = 2.0f * N * dot(light_dir, N);
					glm::vec3 reflectionDir = normalize(scalar - light_dir);
					sumSpecular = sumSpecular + sources[i]->getLightColor() *
						pow(max(0.0f, dot(reflectionDir, -dir)), 200) *
						((double)1 - inShadow);

				}
				hitColor = hitColor + sumDiffuse * hitObj->getColor() *
					hitObj->kd + sumSpecular * hitObj->ks;
				break;
			}
			// default is DIFFUSE_AND_GLOSSY material
			// compute Lambertian (diffuse) and Phong shading		
			default: {
				// add ambient component to color
				hitColor = hitObj->getColor() * opts.ambientLight;
				// iterate thru each light source and sum their contribution
				// kd (diffuse color) * I (light intensity) * dot(N, l) +
				// Ks * I * pow(dot(h, N), phongExponent);
				Color sumDiffuse = Color();
				Color sumSpecular = Color();
				glm::vec3 shadowOrigPoint = (dot(dir, N) < 0) ?
					hitPoint + N * opts.bias : 
					hitPoint - N * opts.bias;
				for (int i = 0; i < sources.size(); i++) {
					float tShadowNear = FLT_MAX;
					Object* shadowObj = nullptr;
					glm::vec3 light_pos;
					// get light direction,  
					glm::vec3 light_dir;
					float light_distance_sq;
					if (sources[i]->getLightType() == AREA_LIGHT) {
						// we want to sample at a randomized position on the 
						// area light =   corner vector (starting pt) +
						// some dist in a + 
						// some dist in b
						 light_pos = sources[i]->getLightPos() + 
							sources[i]->getEdgeA() * jitter.x + 
							sources[i]->getEdgeB() * jitter.y;
						 light_dir = light_pos - hitPoint;
						 // squared distance from hit point to light source
						 light_distance_sq = dot(light_dir, light_dir);
						 light_dir = normalize(light_dir);
					}
					else {
						light_dir = sources[i]->getLightPos() - hitPoint;
						light_distance_sq = dot(light_dir, light_dir);
						light_dir = normalize(light_dir);
					}

					// trace rays back to lightsource and do intersection tests:
					// If shadowRay intersects an object, and the obj is closer
					// to the shadowOrigin than the light, the region will be in shadow
					bool inShadow = trace(shadowOrigPoint, light_dir, objects, 
						tShadowNear, objIndex, uv, &shadowObj) && (tShadowNear * tShadowNear) < light_distance_sq;
					// Also: if the shadow ray is being blocked, and the 
					// object is reflective and refractive (transparent),
					// we brighten up the shadow slightly (as light still passes thru)
					if (shadowObj != nullptr && shadowObj->getMaterialType() == REFLECTION_AND_REFRACTION) {
						sumDiffuse = sumDiffuse + sources[i]->getLightColor() *
							max(0.0f, dot(N, light_dir)) * 0.35f /**((float)1 - inShadow)*/;
					}
					else {
						// calculate diffuse contribution
						// not sure why you need to include the surface color in this equation
						// (1- inShadow) checks if i-th light being blocked by an object
						sumDiffuse = sumDiffuse + sources[i]->getLightColor() *
							max(0.0f, dot(N, light_dir)) * ((double)1 - inShadow);
					}
					// calculate specular contribution
					glm::vec3 scalar = 2.0f * N * dot(light_dir, N);
					glm::vec3 reflectionDir = normalize(scalar - light_dir);
					sumSpecular = sumSpecular + sources[i]->getLightColor() * 
						pow( max(0.0f, dot(reflectionDir, -dir)), hitObj->phongExponent) * 
						((double)1 - inShadow);
				}
				hitColor = hitColor + sumDiffuse * hitObj->getColor() * 
					hitObj->kd + sumSpecular * hitObj->ks;
				break;
			}
		}
	}
	else { // no object intersected
		// return b.g. color
		return hitColor = Color(201.0f / 255.0f, 226.0f / 255.0f, 255.0f/255.0f, .0f);
		//return hitColor = Color(.0f, .0f , .0f, .0f);
	}

	return hitColor.colorClip();
}

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
	glm::vec2& uv, Object** hitObject) {
	
	*hitObject = nullptr;
	int indexK; 
	glm::vec2 uvK;
	float tCurrNearest = FLT_MAX;
	// iterate thru object vector and call intersect on each
	// object, replace saved values with those of the closest obj 
	for (int k = 0; k < objects.size(); k++) {
		
		// intersect will output tCurrNearest 
		// (has freedom to use index k and vector uv also)
		//if (objects[k]->getColor().getColorSpecial() == 2.0f) {
		//	float five = 5.0f;
		//}
		if (objects[k]->findIntersection(orig, dir, tCurrNearest, indexK, uvK)
			&& tCurrNearest < tNear) {
			tNear = tCurrNearest;
			objIndex = k;
			*hitObject = objects[k];
			uv = uvK;
		}
	}
	// if object was hit by ray during intersect test -- returns true
	return (*hitObject != nullptr); 
}

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
		colorBuffer[i] = Color(201.0f / 255.0f, 226.0f / 255.0f, 255.0f / 255.0f, .0f);
	}

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

	// Lights ----------------------------------------------------------
	Light theLight(glm::vec3(-7.0f, 5.0f, 3.0f), 
		whiteLight, POINT_LIGHT, 
		glm::vec3(.0f), glm::vec3(.0f), glm::vec3(.0f));

	// area light
	glm::vec3 edge_a(-5.0f, .0f, .0f);
	glm::vec3 edge_b(0.f, .0f, -5.0f);
	glm::vec3 corner(.0f, 10.0f, -4.0f);
	Light areaLight(corner,
		whiteLight, AREA_LIGHT, corner, edge_a, edge_b);

	// Objects ----------------------------------------------------------
	// center
	Sphere scene_sphere(glm::vec3(.0f, .0f, -3.0f), 1.0f, 
		white, REFLECTION_AND_REFRACTION);
	scene_sphere.ior = 1.04f;
	// right
	Sphere scene_sphere2(glm::vec3(1.7f, -.4f, -2.80f), 
		0.6f, maroon, REFLECTION);
	scene_sphere2.ior = FLT_MAX;
	// left
	Sphere scene_sphere3(glm::vec3(-1.7f, -.4f, -2.80f), 
		0.6f, mint, REFLECTION_AND_REFRACTION);
	scene_sphere3.ior = 3.0f;
	// b.g. mid-left
	Sphere scene_sphere4(glm::vec3(-.4f, -.65f, -5.3f), 0.35f, 
		pastel_pink, DIFFUSE_AND_GLOSSY);
	// b.g. mid-right
	Sphere scene_sphere5(glm::vec3(.4f, -.65f, -5.3f), 0.35f, 
		pastel_blue, DIFFUSE_AND_GLOSSY);
	// foreground left
	Sphere scene_sphere6(glm::vec3(-.5f, -.75f, -1.7f), 0.25f,
		pastel_blue, DIFFUSE_AND_GLOSSY);
	// b.g. left most
	Sphere scene_sphere7(glm::vec3(-3.9f, .0f, -5.3f), 0.35f,
		pastel_pink, DIFFUSE_AND_GLOSSY);

	Plane plane(glm::vec3(.0f, 1.0f, .0f), glm::vec3(1.0f, -1.0f, .0f), floor_white, DIFFUSE_AND_GLOSSY);

	// Populate scene objects ----------------------------------------------------------
	std::vector<Object*> sceneObjects;
	sceneObjects.push_back(&scene_sphere);
	sceneObjects.push_back(&scene_sphere2);
	sceneObjects.push_back(&scene_sphere3);
	sceneObjects.push_back(&scene_sphere4);
	sceneObjects.push_back(&scene_sphere5);
	sceneObjects.push_back(&scene_sphere6);
	sceneObjects.push_back(&scene_sphere7);

	sceneObjects.push_back(&plane);

	// Populate Lights vector ----------------------------------------------------------
	std::vector<LightSources*> lights;
	//lights.push_back(&theLight);
	lights.push_back(&areaLight);

	// Generating Camera ----------------------------------------------------------  
	glm::vec3 cameraPos(.0f, -.2f, 0.0f);
	glm::vec3 cameraForward(.0f, .0f, -1.0f);
	glm::vec3 cameraReferUp(.0f, 1.0f, .0f);
	glm::vec3 cameraRight(1.0f, .0f, .0f);
	Camera cam(cameraPos, cameraForward, cameraReferUp);

	float alpha, beta;
	glm::vec3 rayDir, rayOrigin;
	// anti aliasing color buffer
	Color* tempColor = new Color[options.aaDepth * options.aaDepth];
	int aaIdx = 0; // anti aliasing index
	// generate 2 size n^2 arrays holding randomized values in range [0, 1)
	glm::vec2* r = new glm::vec2[options.sampleNum * options.sampleNum];
	glm::vec2* s = new glm::vec2[options.sampleNum * options.sampleNum];


	// Begin rendering ----------------------------------------------------------
	for (int y = 0; y < options.height; y++) {
		for (int x = 0; x < options.width; x++) {
			Color pixelColor;
			// populate the arrays with values
			for (int idx = 0; idx < options.sampleNum * options.sampleNum; ++idx) {
				float rVal = rand() / RAND_MAX; // generates a value in range [0, 1)
				r[idx].x = ((float)rand()) / RAND_MAX; 
				s[idx].x = ((float)rand()) / RAND_MAX; 
				r[idx].y = /*((float)rand()) / RAND_MAX*/s[idx].x;
				s[idx].y = /*((float)rand()) / RAND_MAX*/r[idx].x;
			}
			// shuffle array s[] -- shirley shuffle method
			// reduce/eliminates coherence between r[] and s[] float values
			// for more randomized shadow noise
			//for (int p = options.sampleNum * options.sampleNum - 1; p > 0 ; --p) {
			//	// choose rand num in [0, p]
			//	float number = ((float) rand() / (float) RAND_MAX) - .001f;
			//	int j = (p + 1) * number;
			//	//std::cout << s[p].x << " " << s[p].y << " and " << s[j].x << " " << s[j].y << std::endl;
			//	std::swap(s[p], s[j]);
			//	//std::swap(r[p], r[j]);
			//	//std::cout << s[p].x << " " << s[p].y << " and " << s[j].x << " " << s[j].y << std::endl;
			//}

			// Render w/o anti-aliasing
			if (options.aaDepth == 1 && options.softShadows == false) {
					alpha = ((2 * (x + .5f) / (float)options.width) - 1.0f)
						* options.aspectRatio * tan(options.fov / 2);
					beta = (1 - (2 * (y + 0.5) / (float)options.height))
						* tan(options.fov / 2);
					rayDir = normalize(glm::vec3(alpha, beta, .0f) + cam.getCamLookAt());
					rayOrigin = cameraPos;
					Ray camRay(rayOrigin, rayDir); // generate cam ray

					// castRay function (replaces the getColor() function)
					// this replaces getColor function
					pixelColor = castRay(rayOrigin, rayDir, lights, sceneObjects, options, STARTING_DEPTH, glm::vec2(.0f));
			}
			else if (options.aaDepth == 1 && options.softShadows == true) {
				for (int l = 0; l < options.sampleNum * options.sampleNum; ++l) {
					// Randomized components to jitter the rays casted 
					// into each pixel
					alpha = ((2 * (x + r[l].x) / (float)options.width) - 1.0f)
						* options.aspectRatio * tan(options.fov / 2);
					beta = (1 - (2 * (y + r[l].y) / (float)options.height))
						* tan(options.fov / 2);
							/*alpha = ((2 * (x + .5f) / (float)options.width) - 1.0f)
								* options.aspectRatio * tan(options.fov / 2);
							beta = (1 - (2 * (y + 0.5) / (float)options.height))
								* tan(options.fov / 2);*/
							rayDir = normalize(glm::vec3(alpha, beta, .0f) + cam.getCamLookAt());
							rayOrigin = cameraPos;
							Ray camRay(rayOrigin, rayDir); // generate cam ray

							// castRay function (replaces the getColor() function)
							// this replaces getColor function
							pixelColor = pixelColor + castRay(rayOrigin, rayDir, lights, sceneObjects, options, STARTING_DEPTH, s[l]);
				}
				// average out the color sampled from n^2 rays cast each indiv. pixel
				// by dividing pixelColor / n^2 
				pixelColor = pixelColor * (float) (1.0f / (float)(options.sampleNum * options.sampleNum));
			}
			// Render with Anti-aliasing 
			else {
				// add another double for loop here for anti-aliasing
				for (int aay = 0; aay < options.aaDepth; aay++) {
					for (int aax = 0; aax < options.aaDepth; aax++) {
						aaIdx = aax + aay * options.aaDepth;
						alpha = ((2 * (x + (float)aax / ((float)options.aaDepth - 1) ) / (float)options.width) - 1.0f)
							* options.aspectRatio * tan(options.fov / 2.0f);
						beta = (1 - (2 * (y + (float)aay / ((float)options.aaDepth - 1)) / (float)options.height)) * tan(options.fov / 2.0f);
						
						rayDir = normalize(glm::vec3(alpha, beta, .0f) + cam.getCamLookAt());
						rayOrigin = cameraPos;
						Ray camRay(rayOrigin, rayDir); // generate cam ray

						// castRay function (replaces the getColor() function)
						// this replaces getColor function
						// getColorAt returns the clipped color
						Color tempCol = castRay(rayOrigin, rayDir, lights, sceneObjects, options, STARTING_DEPTH, glm::vec2(.0f));
						tempColor[aaIdx] = tempCol;
					}
				}

				// average all the R,G,B components
				double avgR = 0;
				double avgG = 0;
				double avgB = 0;
				for (int k = 0; k < options.aaDepth * options.aaDepth; k++) {
					avgR += tempColor[k].getColorR();
					avgG += tempColor[k].getColorG();
					avgB += tempColor[k].getColorB();
				}
				pixelColor.setColorR(avgR / ((double)options.aaDepth * options.aaDepth));
				pixelColor.setColorG(avgG / ((double)options.aaDepth * options.aaDepth));
				pixelColor.setColorB(avgB / ((double)options.aaDepth * options.aaDepth));
			}
			
			// apply the color to the final image
			colorBuffer[x + y * options.width].setColorR(pixelColor.getColorR());
			colorBuffer[x + y * options.width].setColorG(pixelColor.getColorG());
			colorBuffer[x + y * options.width].setColorB(pixelColor.getColorB());
		}
	}

	std::string fileName = "testFile.jpg";
	writeImage(fileName, 1.0, 2.2, colorBuffer, options.width, options.height);
	t2 = clock();
	std::cout << "Render time: " << (float)(t2 - t1) / 1000.0f << " seconds" << std::endl;
	// Free memory
	delete[] colorBuffer;
	delete[] tempColor;
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

float clamp(const float& lo, const float& hi, const float&v) {
	return max(lo, min(hi, v));
}