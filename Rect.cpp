#include "Rect.h"

Rect::Rect()
{
    corner = glm::vec3(.0f);
    edge_1 = glm::vec3(.0f);
    edge_2 = glm::vec3(.0f);
    color = Color(.5f, .5f, .5f, .0f);
    normal = normalize(cross(edge_1, edge_2));
}

Rect::Rect(glm::vec3 c, glm::vec3 e_a, glm::vec3 e_b, Color col, materialType mat)
{
    corner = c; 
    edge_1 = e_a;
    edge_2 = e_b;
    color = col;
    normal = normalize(cross(edge_1, edge_2));
    material = mat;
}

bool Rect::findIntersection(glm::vec3 orig, glm::vec3 dir, float& tNear, int& index, glm::vec2& uv) const
{   
    glm::vec3 N = this->normal;
    // use the plane intersection routine to see if intersects plane
    float denom = dot(dir, N);
    if (fabsf(denom) < 0.0001f) {
        return false;
    }
    // means ray intersects the back face of the plane
    // so flip the normal
    if (dot(dir, N) > 0.0f) {
        N = -N;
    }
    // this->center is the "center" of the plane, 
    // i.e. an Abitrary pt A on the plane
    float numer = dot(this->corner - orig, N);
    tNear = numer / denom;
    if (tNear < 0.0001f) {
        return false;
    }
    // now check if intersection lies within the plane
    // do the projection of PP0 onto edge 1 and 2 and 
    // compare their lengths
    glm::vec3 p0p = (orig + dir * tNear) - corner; // obtaining PP0

    float length_1 = dot(p0p, edge_1) / dot(edge_1, edge_1);
    float length_2 = dot(p0p, edge_2) / dot(edge_2, edge_2);
    float edge_1_len = length(edge_1);
    float edge_2_len = length(edge_2);
    if ((0.0f < length_1 && length_1 <= length(edge_1)) && ((0.0f < length_2) && (length_2 <= length(edge_2))) ) {
        float num = 1.0f;
    }
    // If the length of both projections are within the lengths of the 
    // edge vectors--there will be intersection. If not, no intersection
    return (0.0f < length_1 && length_1 <= length(edge_1)) && ((0.0f < length_2) && (length_2 <= length(edge_2)));
}

Color Rect::getColor()
{
    return color;
}

void Rect::setColor(float r, float g, float b)
{
    color.setColorR(r);
    color.setColorG(g);
    color.setColorB(b);

}

glm::vec3 Rect::getNormal(glm::vec3 point)
{
    return normal;
}

void Rect::getSurfaceProperties(const glm::vec3& P, const glm::vec3& I, const int& index, const glm::vec2& uv, glm::vec3& N, glm::vec2& st)
{
    return;
}

materialType Rect::getMaterialType()
{
    return material;
}
