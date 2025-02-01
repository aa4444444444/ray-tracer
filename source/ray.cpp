#include "../header/ray.h"

Ray::Ray(float originX, float originY, float originZ, float directionX, float directionY, float directionZ)
{
    m_origin = Point(originX, originY, originZ);
    m_direction = Vector(directionX, directionY, directionZ);
    m_direction.normalize();
}

Point Ray::getOrigin() { return m_origin; }

Vector Ray::getDirection() { return m_direction; }
