#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "LightSources.h"
#include "Color.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light : public LightSources {
private:
	glm::vec3 lightPos;
	Color lightColor;
public:

	Light();
	Light(glm::vec3 light_position, Color light_color);

	//virtual glm::vec3 getLightDir(); 1
	glm::vec3 getLightPos();
	Color getLightColor();

};

#endif