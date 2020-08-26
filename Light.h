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
	lightType type;

	glm::vec3 corner;
	glm::vec3 edge_1, edge_2;
public:

	Light();
	Light(glm::vec3 light_position,	Color light_color,
		lightType light_type, glm::vec3 c,
		glm::vec3 a, glm::vec3 b);

	//virtual glm::vec3 getLightDir();
	glm::vec3 getLightPos();
	glm::vec3 setLightPos(glm::vec3 pos);

	Color getLightColor();
	void setLightColor(Color light_col);

	void setLightType(lightType t); 
	lightType getLightType();

	// point light getEdgeA and B do nothing 
	glm::vec3 getEdgeA();
	glm::vec3 getEdgeB();

	//glm::vec3 findIntersection()
};

#endif