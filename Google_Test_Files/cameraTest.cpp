#include "pch.h"
#include "cameraTest.h"

TEST_F(cameraTest, camDirTest) {
  EXPECT_EQ(cam->getCamPos(), glm::vec3(.0f, -.2f, .0f));
  EXPECT_EQ(cam->getCamLookAt(), 
	  normalize(glm::vec3(.0f, .0f, -1.0f) - glm::vec3(.0f, -.2f, .0f)));
  EXPECT_EQ(cam->getCamRight(), 
	  normalize(cross(cam->getCamLookAt(), glm::vec3(0.0f, 1.0f, .0f))));
  EXPECT_EQ(cam->getCamUp(), 
	  normalize(cross(cam->getCamRight(), cam->getCamLookAt())));
}