#include "Box.h"

Box::Box() {
    // default white cube
    color = Color(1.0f, 1.0f, 1.0f, .0f);
    centroid = glm::vec3(.0f);

    float halfLength = 0.25f;
    // min
    bounds[0] = centroid + glm::vec3(-halfLength, -halfLength, halfLength);
    // max
    bounds[1] = centroid + glm::vec3(halfLength, halfLength, -halfLength);
    // build routine for bounding box min, max 
    bbox.minBounds = bounds[0];
    bbox.maxBounds = bounds[1];
}

//Box::Box(glm::vec3 min, glm::vec3 max, Color c, materialType mat) {
//    bounds[0] = min;
//    bounds[1] = max;
//    color = c;
//    material = mat;
//    if (mat == REFLECTION) {
//        ior = FLT_MAX;
//    }
//
//    // build routine for bounding box min, max 
//    bbox.min = min;
//    bbox.max = max;
//}

Box::Box(glm::vec3 centroid, float length, Color c, float refractIdx, materialType mat) {
    ior = refractIdx;
    color = c;
    material = mat;

    // Opaque objects have infinite idx of refrac
    if (mat == REFLECTION ||
        mat == DIFFUSE_AND_GLOSSY ||
        mat == DIFFUSE_AND_GLOSSY_AND_REFLECTION) {
        ior = FLT_MAX;
    }

    // length of one side, but we rely on the centroid 
    // (center of box) for reference value, so halve it
    float halfLength = length * 0.5f;
    // set the min and max bounds 
    // min
    bounds[0] = centroid + glm::vec3(-halfLength, -halfLength, halfLength);
    // max
    bounds[1] = centroid + glm::vec3(halfLength, halfLength, -halfLength);

    // build routine for bounding box min, max 
    bbox.minBounds = bounds[0];
    bbox.maxBounds = bounds[1];
}

bool Box::findIntersection(glm::vec3 orig, glm::vec3 dir, 
    float& tNear, int& index, glm::vec2& uv) const {
    // find tminx and tminy and tmaxX and Y 
    // we want to account for when the direction for any of the 
    // x y z components is == 0 which also saves on calling swap
    // function
    glm::vec3 inverseDir = 1.0f / dir;

    float tminx, tmaxx;
    if (inverseDir.x >= .0f) {
        tminx = (bounds[0].x - orig.x) * inverseDir.x;
        tmaxx = (bounds[1].x - orig.x)* inverseDir.x;
    }
    else {
        tminx = (bounds[1].x - orig.x) * inverseDir.x;
        tmaxx = (bounds[0].x - orig.x) * inverseDir.x;
    }


    float tminy, tmaxy;
    if (inverseDir.y >= .0f) {
        tminy = (bounds[0].y - orig.y) * inverseDir.y;
        tmaxy = (bounds[1].y - orig.y) * inverseDir.y;
    }
    else {
        tminy = (bounds[1].y - orig.y) * inverseDir.y;
        tmaxy = (bounds[0].y - orig.y) * inverseDir.y;
    }
    //float tminx = (bounds[0].x - orig.x) / dir.x;
    //float tmaxx = (bounds[1].x - orig.x) / dir.x;
    //// in some cases, tmin is greater than tmax depending
    //// on the ray dir, so we swap these values
    //if (tminx > tmaxx) std::swap(tminx, tmaxx);

    //float tminy = (bounds[0].y - orig.y) / dir.y;
    //float tmaxy = (bounds[1].y - orig.y) / dir.y;
    //// same check for which is greater tmin or tmax
    //if (tminy > tmaxy) std::swap(tminy, tmaxy);

    // check if the ray totally miss x-y planes
    if ((tminx > tmaxy) || (tminy > tmaxx)) 
        return false;

    // we take the largest of the minimum
    float tmin = tminx < tminy ? tminy : tminx;
    // we take the smallest of the maximum
    float tmax = tmaxx > tmaxy ? tmaxy : tmaxx;

    // after finding max and min re: x-y plane
    // compare tmin and tmax to z max and min now
    //float tminz = (bounds[0].z - orig.z) / dir.z;
    //float tmaxz = (bounds[1].z - orig.z) / dir.z;
    //// same check as before
    //if (tmin > tmax) std::swap(tmin, tmax);

    float tminz, tmaxz;
    if (inverseDir.z >= .0f) {
        tminz = (bounds[1].z - orig.z) * inverseDir.z;
        tmaxz = (bounds[0].z - orig.z) * inverseDir.z;
    }
    else {
        tminz = (bounds[0].z - orig.z) * inverseDir.z;
        tmaxz = (bounds[1].z - orig.z) * inverseDir.z;
    }
    //if (tminz > tmaxz) std::swap(tminz, tmaxz);

    // Check if intersection goes misses z planes
    if ((tmin > tmaxz) || (tmax < tminz)) 
        return false;

    // update tmin and tmax values:
    tmin = tmin < tminz ? tminz : tmin;
    tmax = tmax > tmaxz ? tmaxz : tmax;

    // the intersection point is the ray's entry pt
    if (tmin > 0.0f) {
        tNear = tmin;
    }
    else {
        // if you're casting the shadowRay from behind the box, 
        // and you do intersection test with the same box, the returned 
        // t value will be negative, which means no hit
        return false;
    }

    return true;
}
// Basically the ACTUAL "getNormal()" function: 
// calculates surface normal and stores it
// explanation is found here: 
/* https://blog.johnnovak.net/2016/10/22/
   the-nim-raytracer-project-part-4-calculating-box-normals */
// P -- intersection Pt 
// orig -- ray Origin 
// I -- incoming ray Dir 
// index -- object idx in array
// N (output) -- stores calculated surface normal
// returns the normal of the plane that the ray intersects
void Box::getSurfaceProperties(const glm::vec3& P, const glm::vec3 orig, 
    const glm::vec3& I, const int& index, const glm::vec2& uv, 
    glm::vec3& N, glm::vec2& st) {

    // Get center 
    glm::vec3 center((bounds[0].x + bounds[1].x) / 2.0f,
        (bounds[0].y + bounds[1].y) / 2.0f,
        (bounds[0].z + bounds[1].z) / 2.0f);
    // get vector from center to hit point
    glm::vec3 Pt = P - center;
    // calculate x y and z widths from center
    float dx = fabsf(bounds[0].x - bounds[1].x) / 2.0f;
    float dy = fabsf(bounds[0].y - bounds[1].y) / 2.0f;
    float dz = fabsf(bounds[0].z - bounds[1].z) / 2.0f;
    float bias = 1.001f;

    // get ratio between them and return that as the normal: 
    // theres always atleast 1 component in the internal vector that
    // has a value of 1
    // we multiply by bias to nudge the pts eg. ard 9.999994  up to 1.0f
    N = normalize(glm::vec3((int)(Pt.x / dx * bias), 
        (int)(Pt.y / dy * bias), 
        (int)(Pt.z / dz * bias)));

    // If incoming ray is from inside the obj surface
    // flip normal "inwards"
    N = (dot(N, I) > 0.0f) ? -N : N;

}

Color Box::getColor()
{
    return color;
}

void Box::setColor(float r, float g, float b)
{
    color.setColorR(r);
    color.setColorR(g);
    color.setColorR(b);
}

// param: the intersection point
glm::vec3 Box::getNormal(glm::vec3 point)
{
    return normal;
}

materialType Box::getMaterialType()
{
    return material;
}
