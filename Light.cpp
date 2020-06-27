#include "Light.h"


Light::Light() {
	lightPos = glm::vec3(.0f, 10.0f, .0f);
	//lightDir = glm::vec3(1.0f, .0f, .0f);
	lightColor = Color(1.0f, 1.0f, 1.0f, 0.0f);
}

Light::Light(glm::vec3 light_position, Color light_color) {
	lightPos = light_position;
	//lightDir = light_direction;
	lightColor = light_color;
}

Color Light::getLightColor() {
	return lightColor;
}

glm::vec3 Light::getLightPos() {
	return lightPos;
}

//glm::vec3 Light::getLightDir() {
//	return lightDir;
//}
