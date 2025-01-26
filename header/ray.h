#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vector.h"

class Ray {
public:
    Ray(float originX, float originY, float originZ, float directionX, float directionY, float directionZ);
    ~Ray();
    Point* getOrigin();
    Vector* getDirection();

private:
    Point* m_origin;
    Vector* m_direction;
};

#endif