#include "Camera.h"

Camera::Camera() {
	this->pos = glm::vec3(0.0f);
	this->lookAt = glm::vec3(.0f, .0f, 1.0f);
	this->up = glm::vec3(.0f, 1.0f, .0f);
	// cross prod. use RHR
	this->right = normalize(cross(lookAt, this->up)); 
}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 referUp) {
	this->pos = position;
	this->lookAt = normalize(target - this->pos);
	this->right = normalize(cross(lookAt, referUp));
	this->up = normalize(cross(this->right, lookAt));
}

glm::vec3 Camera::getCamPos() {
	return this->pos;
}

glm::vec3 Camera::getCamLookAt() {
	return this->lookAt;
}

glm::vec3 Camera::getCamUp() {
	return this->up;
}

glm::vec3 Camera::getCamRight() {
	return this->right;
}

Ray Camera::generateCamRay(glm::vec3 camPos, glm::vec3 camDir)
{
	Ray cam(camPos, normalize(camDir));
	return cam;
}
