#ifndef UTIL_H
#define UTIL_H

#include "ray.h"
#include "vector.h"

Vector findReflection(Vector vec, Vector normal)
{
    Eigen::Vector3d reflection = vec.getVector()
        - (2 * vec.dot(&normal)) / (normal.magnitude() * normal.magnitude() * 1.0) * normal.getVector();
    return Vector(reflection);
}

#endif