#ifndef UTIL_H
#define UTIL_H

#include "ray.h"
#include "vector.h"

Vector findReflection(Vector vec, Vector normal);

Vector findTransmission(Vector incoming, Vector normal, float index1, float index2);

float randomBetweenInclusive(float upperBound);

#endif