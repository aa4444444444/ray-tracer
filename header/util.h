#ifndef UTIL_H
#define UTIL_H

#include "ray.h"
#include "vector.h"

Vector findReflection(Ray* ray, Vector normal)
{
    Vector s = ray->getDirection();
    Eigen::Vector3d reflection
        = s.getVector() - (2 * s.dot(&normal)) / (normal.magnitude() * normal.magnitude() * 1.0) * normal.getVector();
    return Vector(reflection);
}

#endif