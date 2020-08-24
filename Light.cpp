#include "Light.h"


Light::Light() {
	lightPos = glm::vec3(.0f, 10.0f, .0f);
	//lightDir = glm::vec3(1.0f, .0f, .0f);
	lightColor = Color(1.0f, 1.0f, 1.0f, 0.0f);
	// lights will be pt lights by default
	type = POINT_LIGHT; 

	corner = glm::vec3(.0f);
	edge_1 = glm::vec3(.0f);
	edge_2 = glm::vec3(.0f);

}

Light::Light(glm::vec3 light_position, 
	Color light_color, 
	lightType light_type, 
	glm::vec3 c,
	glm::vec3 a, 
	glm::vec3 b) {
	
	lightPos = light_position;
	lightColor = light_color;
	type = light_type;

	corner = c;
	edge_1 = a;
	edge_2 = b;
}

Color Light::getLightColor() {
	return lightColor;
}

void Light::setLightColor(Color light_col)
{
	lightColor = light_col;
}

void Light::setLightType(lightType t)
{
	type = t;
}

lightType Light::getLightType()
{
	return type;
}

glm::vec3 Light::getEdgeA()
{
	return edge_1;
}

glm::vec3 Light::getEdgeB()
{
	return edge_2;
}

glm::vec3 Light::setLightPos(glm::vec3 pos)
{
	return lightPos = pos;
}

glm::vec3 Light::getLightPos() {
	return lightPos;
}