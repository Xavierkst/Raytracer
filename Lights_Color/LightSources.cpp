#include "LightSources.h"

Color LightSources::getLightColor()
{
    return Color(.0f, .0f, .0f, .0f);
}

glm::vec3 LightSources::getLightPos()
{
    return glm::vec3(.0f);
}

lightType LightSources::getLightType()
{
    return AREA_LIGHT;
}

glm::vec3 LightSources::getEdgeA()
{
    return glm::vec3(.0f);
}

glm::vec3 LightSources::getEdgeB()
{
    return glm::vec3(.0f);
}
