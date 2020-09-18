#include "Render.h"

void Render::startRender(std::vector<LightSources*>& lights,
	std::vector<Object*>& sceneObjects, 
	Color* colorBuffer, Camera cam,
	Options options, glm::vec2* r, glm::vec2* s) {

	// I would construct the AccelStruct grid 
	// (grid sizing/grid division/populate it etc.) 
	// before casting rays into the scene
	
	// create the grid
	//Grid sceneGrid(sceneObjects, lights);

	// now that my grid is ready, we start casting
	// rays into the scene and traversing the grid
	for (int y = 0; y < options.height; y++) {
		for (int x = 0; x < options.width; x++) {
			Color pixelColor;
			// Generate jittered components for camera and shadow rays
			for (int idx = 0; 
				idx < options.sampleNum * options.sampleNum; ++idx) {
				// generates a value in range [0, 1)
				s[idx].x = r[idx].x = ((float)rand()) / RAND_MAX;
				s[idx].y = r[idx].y = ((float)rand()) / RAND_MAX;
			}
			// shuffle array s[] -- shirley shuffle method
			// reduce/eliminates coherence between r[] and s[] float values
			// for more randomized shadow noise
			if (options.sampleNum > 1) {
				shuffleFloatArray(s, options.sampleNum);
			}
			float alpha, beta;
			glm::vec3 rayDir, rayOrigin;
			// Render with anti-aliasing & soft shadows
			if (options.softShadows == true) {
				for (int l = 0; 
					l < options.sampleNum * options.sampleNum; ++l) {
					// Jitter the rays casted into each pixel
					alpha = ((2 * (x + r[l].x) / (float)options.width) - 1.0f)
						* options.aspectRatio * tan(options.fov / 2);
					beta = (1 - (2 * (y + r[l].y) / (float)options.height))
						* tan(options.fov / 2);
					rayDir = normalize(glm::vec3(alpha, beta, .0f) +
						cam.getCamLookAt());
					rayOrigin = cam.getCamPos();
					Ray camRay(rayOrigin, rayDir); // generate cam ray

					// Cast ray into the scene
					pixelColor = pixelColor + castRay(rayOrigin, rayDir,
						lights, sceneObjects, options, STARTING_DEPTH, s[l]);
				}
				// average out the color sampled from 
				// n^2 rays cast each indiv. pixel
				// by dividing pixelColor / n^2 
				pixelColor = pixelColor *
					(float)(1.0f / 
						(float)(options.sampleNum * options.sampleNum));
			}
			// Render w/o anti-aliasing & soft shadows
			else if (options.softShadows == false) {
				alpha = ((2 * (x + .5f) / (float)options.width) - 1.0f)
					* options.aspectRatio * tan(options.fov / 2);
				beta = (1 - (2 * (y + 0.5) / (float)options.height))
					* tan(options.fov / 2);
				rayDir = normalize(glm::vec3(alpha, beta, .0f) +
					cam.getCamLookAt());
				rayOrigin = cam.getCamPos();
				Ray camRay(rayOrigin, rayDir); // generate cam ray

				// castRay function (replaces the getColor() function)
				// this replaces getColor function
				pixelColor = castRay(rayOrigin, rayDir, lights,
					sceneObjects, options, STARTING_DEPTH, glm::vec2(.0f));
			}

			// write the color to the (i,j)-th pixel in the image buffer
			setPixelColor(x, y, colorBuffer, options.width,
				pixelColor.getColorR(),
				pixelColor.getColorG(),
				pixelColor.getColorB());
		}
	}
}

void Render::setPixelColor(int i, int j, Color* buffer, 
	int width, double r, double g, double b) {

	buffer[i + j * width].setColorR(r);
	buffer[i + j * width].setColorG(g);
	buffer[i + j * width].setColorB(b);
}

Color Render::castRay(const glm::vec3& orig, const glm::vec3& dir, 
	const std::vector<LightSources*>& sources, 
	const std::vector<Object*>& objects, 
	const Options opts, uint32_t depth, 
	glm::vec2 jitter)
{
	Color hitColor = Color();
	// Stopping Condition: 
	// check if depth is greater than maxDepth, if yes, 
	// return no color contribution in this call level
	if (depth > MAX_RECURSION_DEPTH) {
		return hitColor = Color(.0f, .0f, .0f, .0f);
	}

	int objIndex; // stores idx of closest obj in scene list
	glm::vec2 uv; 
	Object* hitObj = nullptr;
	float tNear = FLT_MAX;

	// trace() finds the closest object intersected 
	// we can use a gridTraversal algorithm here to replace trace()
	// function. Instead of using the trace() method to find closest 
	// object intersected in the scene, we will now use our "while(1)"
	// loop in Grid::intersect() to traverse thru the grid cells, returning
	// "true" once first (closest) obj intersected. If not, no object 
	// intersected, return false. 
	if (trace(orig, dir, objects, tNear, objIndex, uv, &hitObj)) {
		// intersection pt & ptr to object has been found
		glm::vec3 hitPoint = orig + dir * tNear;
		glm::vec3 N; // normal
		glm::vec2 st; // for triangle meshes
		glm::vec3 reflection_dir;
		glm::vec3 reflection_ray_origin;
		
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
		hitObj->getSurfaceProperties(hitPoint, orig, dir, objIndex, uv, N, st);

		switch (hitObj->getMaterialType()) {
		case LIGHT: {
			hitColor = hitColor + hitObj->getColor();
		}
		case REFLECTION_AND_REFRACTION: {
			float kr = 0.0f; // reflected light ratio
			float kt; // transmitted light ratio
			// When primary ray incident on transparent material, 
			// 2 rays produced: Reflection and refraction ray

			// Refraction ray: bias, check if incoming ray from 
			// inside or outside surface
			glm::vec3 refract_ray_orig = dot(dir, N) < 0.0f ?
				hitPoint - N * opts.bias : hitPoint + N * opts.bias;
			glm::vec3 refract_ray_dir =
				normalize(refract(dir, N, hitObj->ior));
			// compute fresnel
			fresnel(dir, N, hitObj->ior, kr);
			// Proportion of light transmitted 
			kt = 1.0f - kr;
			// if amt of light reflected is less than 100% 
			if (kr < 1.0f) {
				// cast refraction ray:
				hitColor = hitColor + hitObj->getColor() *
					castRay(refract_ray_orig, refract_ray_dir,
						sources, objects, opts, ++depth, jitter) * kt;
			}

			// Generate reflection ray: 
			reflect(dir, N, hitPoint,
				reflection_ray_origin, reflection_dir, opts);
			hitColor = hitColor +
				castRay(reflection_ray_origin, reflection_dir,
					sources, objects, opts, ++depth, jitter) * kr;
			break;
		}
		case REFLECTION: { // object is perfectly a mirror
			float kr = .0f;
			// fresnel -- sets the normal, and sets Kr (reflect ratio)
			fresnel(dir, N, hitObj->ior, kr);

			// compute reflection direction
			reflect(dir, N, hitPoint,
				reflection_ray_origin, reflection_dir, opts);

			// make recursive call to castRay function to sample the color of 
			// the reflected ray cast out from the hitPoint 
			hitColor = hitColor +
				castRay(reflection_ray_origin, reflection_dir,
					sources, objects, opts, ++depth, jitter) * kr;
			break;
		}
		case DIFFUSE_AND_GLOSSY_AND_REFLECTION: {
			// REFLECTIVE PART -- apply recursion
			float kr = .0f;
			// fresnel -- sets the normal, and sets Kr (reflect ratio)
			fresnel(dir, N, hitObj->ior, kr);

			// compute reflection direction
			reflect(dir, N, hitPoint,
				reflection_ray_origin, reflection_dir, opts);
			// make recursive call to castRay function to sample the color of 
			// the reflected ray cast out from the hitPoint 
			hitColor = hitColor +
				castRay(reflection_ray_origin, reflection_dir,
					sources, objects, opts, ++depth, jitter) * kr;

			// Apply phong shading
			hitColor = hitColor + phongShading(dir, N, hitPoint,
				hitObj, sources, objects, opts, jitter);
			break;
		}
		// default material is DIFFUSE_AND_GLOSSY 
		// Apply Blinn Phong shading		
		case DIFFUSE_AND_GLOSSY: {
			// apply Phong shading
			hitColor = hitColor + phongShading(dir, N, hitPoint,
				hitObj, sources, objects, opts, jitter);
			break;
		}
		default: {
			// current default material is DIFFUSE
			// apply Phong shading -- the summation already
			// done inside
			hitColor = hitColor + phongShading(dir, N, hitPoint,
				hitObj, sources, objects, opts, jitter);
			break;
		}
		}
	}
	else {
		// no object intersected, return b.g. color
		return hitColor = opts.backgroundColor;
	}
	// clip color incase value exceeds 1.0f in any component
	return hitColor.colorClip();
}

void Render::shuffleFloatArray(glm::vec2* s, int sampleNum)
{
	for (int p = sampleNum * sampleNum - 1; p > 0; --p) {
		// choose rand num in [0, p]
		float number = ((float)rand() / (float)RAND_MAX) - .001f;
		int j = (p + 1) * number;
		std::swap(s[p], s[j]);
	}
}

bool Render::trace(glm::vec3 orig, glm::vec3 dir, 
	const std::vector<Object*>& objects, 
	float& tNear, int& objIndex, glm::vec2& uv, 
	Object** hitObject) {

	*hitObject = nullptr;
	int indexK;
	glm::vec2 uvK;
	float tCurrNearest = FLT_MAX;
	// iterate thru object vector and call intersect on each
	// object, replace saved values with those of the closest obj 
	for (int k = 0; k < objects.size(); k++) {

		// findIntersectionwill save tCurrNearest, and pointer to nearest
		// object
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

float Render::clamp(const float& lo, const float& hi, const float& v) {
	return max(lo, min(hi, v));
}

void Render::fresnel(const glm::vec3& I, glm::vec3& N, float& ior, float& kr) {
	// get the refraction indices and eta 
	float n1 = 1.0f; float n2 = ior;
	float cosi = clamp(-1, 1, dot(I, N));

	// check if ray is incoming from inside or outside the object
	// dot(I, N) >= 0 means coming from inside, so we swap the 
	// ref. indices, also flip the normal
	if (cosi > 0.0f) {
		std::swap(n1, n2);
		//nRef = -N; // no need to swap normal
	}
	float eta = n1 / n2;
	// check for total internal reflection, 
	// i.e. if 1 - sin(theta2)^2 term is negative
	// (or) sin(theta2) > 1, then its TIR
	float sint = eta * sqrtf(max(.0f, (1.0f - cosi * cosi)));
	// either the critical angle is surpassed (sint>=1.0f) or 
	// obj's refractive index is infinity, i.e. obj is opaque. c/v = Infinity
	if (sint >= 1.0f || n2 == FLT_MAX) {
		// TIR occurs 
		kr = 1.0f;
	}
	// if NOT TIR, theres some portion of 
	// incident light that gets transmitted
	else {
		float cost = sqrtf(max(0.0f, sqrtf(1 - sint * sint)));
		cosi = fabsf(cosi);
		float Rs = ((n1 * cosi) - (n2 * cost)) / ((n1 * cosi) + (n2 * cost));
		float Rp = ((n2 * cosi) - (n1 * cost)) / ((n2 * cosi) + (n1 * cost));

		kr = 0.5f * ((float)Rp * Rp + (float)Rs * Rs);
	}
}

glm::vec3 Render::refract(glm::vec3 I, glm::vec3 N, float ior) {
	// we assume incoming ray by default comes from the medium
// of lower refractive index (i.e. from air to something denser)
	float cosi = clamp(-1, 1, dot(I, N));
	float n1 = 1.0f; float n2 = ior;
	glm::vec3 nRef = N;
	// test if ray is coming from medium of lower/higher ref idx
	if (cosi < 0) {
		// ray coming from medium of lower ref idx,
		// cosi or cos(theta 1) will be negative, so negate that
		cosi = -cosi;
	}
	else {
		// coming from denser to less dense medium. Swap refract idxes
		std::swap(n1, n2);
		// negate the normal as well. 
		// dw about cosi, its unchanged since >= 0 alrdy
		nRef = -N;
	}

	float eta = n1 / n2;
	// check for total internal reflection TIR -- see if  1- sin(theta2)^2
	// +ve or -ve, if negative, no portion of ray is transmitted
	float k = 1 - eta * eta * (1 - cosi * cosi);
	return k < 0.0f ? glm::vec3(.0f) : 
		eta * I + nRef * (eta * cosi - sqrtf(k));
}

void Render::reflect(glm::vec3 ray_dir, glm::vec3 N, 
	glm::vec3 hitPoint, glm::vec3& reflect_orig, 
	glm::vec3& reflect_dir, Options opts) {

	reflect_dir = normalize(ray_dir - 2.0f * dot(ray_dir, N) * N);
	// reflection_ray_origin biased to avoid shadow acne
	// if dot(R, N) < 0, ref ray inside the surface, else its outside surface
	reflect_orig = (dot(reflect_dir, N) < 0.0f) ?
		hitPoint - N * opts.bias :
		hitPoint + N * opts.bias;
}

Color Render::phongShading(const glm::vec3 dir, const glm::vec3 N, 
	const glm::vec3 hitPoint, Object* hitObj, 
	const std::vector<LightSources*>& sources, 
	const std::vector<Object*>& objects, 
	const Options& opts, glm::vec2& jitter) {

	// iterate thru each light source and sum their contribution
	// kd (diffuse color) * I (light intensity) * dot(N, l) +
	// Ks * I * pow(dot(h, N), phongExponent);
	Color sumDiffuse = Color(); // initialized to black
	Color sumSpecular = Color(); // initialized to black
	glm::vec3 shadowOrigPoint = (dot(dir, N) < 0) ?
		hitPoint + N * opts.bias :
		hitPoint - N * opts.bias;
	for (int i = 0; i < sources.size(); i++) {
		float tShadowNear = FLT_MAX;
		Object* shadowObj = nullptr;
		glm::vec3 light_pos;
		glm::vec3 light_dir;
		float light_distance_sq;
		if (sources[i]->getLightType() == AREA_LIGHT) {
			// we want to sample at a randomized position on the 
			// area light = corner vector (starting pt) +
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
			// get light direction,  
			light_dir = sources[i]->getLightPos() - hitPoint;
			// squared distance from hit point to light source
			light_distance_sq = dot(light_dir, light_dir);
			light_dir = normalize(light_dir);
		}
		// trace rays back to lightsource and do intersection tests:
		// If an object intersected by shadow ray, and the object is closer
		// to the shadowOrigin than the light, the region will be in shadow
		glm::vec2 uv;
		int objIndex;
		bool inShadow = trace(shadowOrigPoint, light_dir, objects,
			tShadowNear, objIndex, uv, &shadowObj) && 
			(tShadowNear * tShadowNear) < light_distance_sq;

		// calculate diffuse contribution
		// not sure why you need to include the surface color in this equation
		// (1- inShadow) checks if i-th light being blocked by an object
		// We lighten the shadows for objects that 
		// are transparent (or semi-transp)
		if (inShadow && 
			shadowObj->getMaterialType() == REFLECTION_AND_REFRACTION) {

			sumDiffuse = sumDiffuse + sources[i]->getLightColor() *
				max(0.0f, dot(N, light_dir)) * 0.35f;
		}
		else {
			sumDiffuse = sumDiffuse + sources[i]->getLightColor() *
				max(0.0f, dot(N, light_dir)) * ((double)1 - inShadow);
		}
		if (hitObj->getMaterialType() != DIFFUSE) { // non diffuse materials have specular
			// calculate specular contribution
			glm::vec3 scalar = 2.0f * N * dot(light_dir, N);
			glm::vec3 reflectionDir = normalize(scalar - light_dir);
			sumSpecular = sumSpecular + sources[i]->getLightColor() *
				pow(max(0.0f, dot(reflectionDir, -dir)), 200) *
				((double)1 - inShadow);
		}
	}

	// sum up the 3 color components 
	return hitObj->getColor() * opts.ambientLight + // ambient
		sumDiffuse * hitObj->getColor() * hitObj->kd + // diffuse
		sumSpecular * hitObj->ks; // specular 
}
void Render::writeImage(std::string fileName, float exposure,
	float gamma, Color* pixelData, int width, int height) {

	std::vector<unsigned char> imageData(width * height * 4);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Color pixelColor = pixelData[x + y * width];
			int index = 4 * (x + y * width);
			imageData[index + 0] =
				(unsigned char)(pixelColor.getColorR() * 255.0f);
			imageData[index + 1] =
				(unsigned char)(pixelColor.getColorG() * 255.0f);
			imageData[index + 2] =
				(unsigned char)(pixelColor.getColorB() * 255.0f);
			imageData[index + 3] = 255.0f; // alpha channel
		}
	}
	std::cout << imageData.size() << std::endl;
	std::cout << height << " " << width << std::endl;
	resultToPNG(fileName, width, height, imageData);
}