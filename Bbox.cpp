#include "Bbox.h"

// since our camera/eye faces -z Axis:
// for z values, the minimal value is small-negative, while 
// the maximum value is large-negative
Bbox::Bbox() : maxBounds(-FLT_MAX, FLT_MAX, FLT_MAX), minBounds(FLT_MAX, -FLT_MAX, -FLT_MAX) {
	// puts min and max as infinities by default
    // so their values would def be assigned upon
    // initialization
}

Bbox::Bbox(glm::vec3 minBound, glm::vec3 maxBound) : 
	maxBounds(maxBound), minBounds(minBound) {
}

// increases the grid's size accordingly by merging
// all the scene's object's bounding boxes -- iterates thru 
// every one of them to do so
Bbox& Bbox::extendBy(glm::vec3& Pt) {
    // we do infinity checks for x and z axis to filter out
    // finite bounds for infinite planes

	//// compare this Bbox's max and min values
	//// to the max and mins passed into it 
	//// and assume their values if lesser than the max
	//// or greater than the min
	//if (Pt.x != -FLT_MAX && min.x > Pt.x) min.x = Pt.x;
	//if (min.y > Pt.y) min.y = Pt.y;
    // since our eye faces -z Axis:
    // for z values, the minimal value is small-negative, while 
    // the maximum value is large-negative
	//if (Pt.z != FLT_MAX && min.z < Pt.z) min.z = Pt.z;
	//if (Pt.x != FLT_MAX && max.x < Pt.x) max.x = Pt.x;
	//if (max.y < Pt.y) max.y = Pt.y;
	//if (Pt.z != -FLT_MAX && max.z > Pt.z) max.z = Pt.z;

    if (minBounds.x > Pt.x) minBounds.x = Pt.x;
    if (minBounds.y < Pt.y) minBounds.y = Pt.y;
    if (minBounds.z < Pt.z) minBounds.z = Pt.z;
    if (maxBounds.x < Pt.x) maxBounds.x = Pt.x;
    if (maxBounds.y > Pt.y) maxBounds.y = Pt.y;
    if (maxBounds.z > Pt.z) maxBounds.z = Pt.z;

	// returns a reference to the same Bbox
	return *this;
}

// Same as Box class intersection routine
// returns true if bounding box is intersected 
// orig -- ray origin
// dir -- ray direction
// tNear -- nearest parametric distance
bool Bbox::findIntersection(glm::vec3 orig, glm::vec3 dir, float& tNear) {
    glm::vec3 inverseDir = 1.0f / dir;
    // IEEE floating point counts -.0f == .0f as TRUE
    // so we must account for this error
    float tminx, tmaxx;
    if (inverseDir.x >= .0f) {
        tminx = (minBounds.x - orig.x) * inverseDir.x;
        tmaxx = (maxBounds.x - orig.x) * inverseDir.x;
    }
    else {
        tminx = (maxBounds.x - orig.x) * inverseDir.x;
        tmaxx = (minBounds.x - orig.x) * inverseDir.x;
    }


    float tminy, tmaxy;
    if (inverseDir.y >= .0f) {
        tminy = (maxBounds.y - orig.y) * inverseDir.y;
        tmaxy = (minBounds.y - orig.y) * inverseDir.y;
    }
    else {
        tminy = (minBounds.y - orig.y) * inverseDir.y;
        tmaxy = (maxBounds.y - orig.y) * inverseDir.y;
    }

    // check if the ray totally miss x-y planes
    if ((tminx > tmaxy) || (tminy > tmaxx))
        return false;

    // we take the largest of the minimum
    float tmin = tminx < tminy ? tminy : tminx;
    // we take the smallest of the maximum
    float tmax = tmaxx > tmaxy ? tmaxy : tmaxx;

    // after finding max and min re: x-y plane
    // compare tmin and tmax to z max and min now
    float tminz, tmaxz;
    if (inverseDir.z >= .0f) {
        tminz = (maxBounds.z - orig.z) * inverseDir.z;
        tmaxz = (minBounds.z - orig.z) * inverseDir.z;
    }
    else {
        tminz = (minBounds.z - orig.z) * inverseDir.z;
        tmaxz = (maxBounds.z - orig.z) * inverseDir.z;
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

glm::vec3 Bbox::getCentroid()
{
    return (maxBounds + minBounds) * 0.5f;
}           

