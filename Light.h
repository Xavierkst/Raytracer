#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Color.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light {
private:
	glm::vec3 lightPos;
	Color lightColor;
public:

	Light();
	Light(glm::vec3 light_position, Color light_color);

	//virtual glm::vec3 getLightDir();
	glm::vec3 getLightPos();
	Color getLightColor();

};

#endif