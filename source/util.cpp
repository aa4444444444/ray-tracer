#include "../header/util.h"

// NORMAL SHOULD BE NORMALIZED!!!!!!
Vector findReflection(Vector vec, Vector normal)
{
    normal.normalize();
    Eigen::Vector3d reflection = vec.getVector() - (2 * vec.dot(&normal)) * normal.getVector();
    return Vector(reflection);
}

// Returns a float between 0.0 and the upper bound inclusive
float randomBetweenInclusive(float upperBound)
{
    return upperBound * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}