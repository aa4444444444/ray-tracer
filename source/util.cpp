#include "../header/util.h"
#include <cmath>

// NORMAL SHOULD BE NORMALIZED!!!!!!
Vector findReflection(Vector vec, Vector normal)
{
    normal.normalize();
    Eigen::Vector3d reflection = vec.getVector() - (2 * vec.dot(&normal)) * normal.getVector();
    return Vector(reflection);
}

Vector findTransmission(Vector incoming, Vector normal, float index1, float index2)
{

    incoming.normalize();
    normal.normalize();
    bool inside = normal.dot(&incoming) >= 0;

    float startingIndex = inside ? index2 : index1;
    float endingIndex = inside ? index1 : index2;

    Vector adjustedNormal = Vector(normal);

    if (inside) {
        adjustedNormal.scale(-1.0);
    }

    float indexRatio = startingIndex / endingIndex;
    Vector negativeIncoming = Vector(incoming);
    negativeIncoming.scale(-1.0f);

    float cosine = negativeIncoming.dot(&adjustedNormal);
    float sineSquared = pow(indexRatio, 2) * (1.0f - cosine * cosine);

    if (sineSquared > 1.0f) {
        // Total Internal Reflection
        // Transmission Ray should be spawned in the reflection direction
        return findReflection(incoming, normal);
    }

    Vector transmissionVector = Vector(incoming);
    transmissionVector.scale(indexRatio);
    adjustedNormal.scale(indexRatio * cosine - sqrt(1.0f - sineSquared));
    transmissionVector.translate(adjustedNormal.getVector());
    return transmissionVector;
}

// Returns a float between 0.0 and the upper bound inclusive
float randomBetweenInclusive(float upperBound)
{
    return upperBound * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}