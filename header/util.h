#ifndef UTIL_H
#define UTIL_H

#include "ray.h"
#include "vector.h"

// NORMAL SHOULD BE NORMALIZED!!!!!!
Vector findReflection(Vector vec, Vector normal)
{
    Eigen::Vector3d reflection = vec.getVector() - (2 * vec.dot(&normal)) * normal.getVector();
    return Vector(reflection);
}

#endif