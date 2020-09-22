#pragma once

#include "gtest/gtest.h"
#include "glm/glm.hpp"
#include "../Camera_Ray/Camera.h"
#include "../Camera_Ray/Camera.cpp"

class cameraTest : public testing::Test {
private: 

public: 

	Camera* cam;

	cameraTest() {
		glm::vec3 cameraPos(.0f, -.2f, 0.0f);
		glm::vec3 cameraForward(.0f, .0f, -1.0f);
		glm::vec3 cameraReferUp(.0f, 1.0f, .0f);
		glm::vec3 cameraRight(1.0f, .0f, .0f);
		cam = new Camera(cameraPos, cameraForward, cameraReferUp);
	}

	~cameraTest() {
		delete cam;
	}

};


