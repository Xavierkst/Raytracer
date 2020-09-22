#ifndef _CAMERA_H
#define _CAMERA_H

#include "Ray.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	// camera has a: pos, lookAt, Up, 
	// looking in the -Z direction? 
	glm::vec3 getCamPos();
	glm::vec3 getCamLookAt();
	glm::vec3 getCamUp();
	glm::vec3 getCamRight();

	Camera();
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 referUp);
private:
	glm::vec3 pos, lookAt, up, right;


};

#endif