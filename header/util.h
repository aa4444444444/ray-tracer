#ifndef UTIL_H
#define UTIL_H

#include "ray.h"
#include "vector.h"

Vector findReflection(Vector vec, Vector normal);

float randomBetweenInclusive(float upperBound);

#endif