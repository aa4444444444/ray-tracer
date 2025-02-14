#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vector.h"

/**
 * @brief Represents a ray in 3D space.
 *
 * Rays have an origin and a direction.
 */
class Ray {
public:
    Ray(float originX, float originY, float originZ, float directionX, float directionY, float directionZ);
    Ray(Point p, Vector v);
    Point getOrigin();
    Vector getDirection();

private:
    Point m_origin;
    Vector m_direction;
};

#endif