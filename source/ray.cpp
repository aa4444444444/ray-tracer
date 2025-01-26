#include "../header/ray.h"

Ray::Ray(float originX, float originY, float originZ, float directionX, float directionY, float directionZ)
{
    m_origin = new Point(originX, originY, originZ);
    m_direction = new Vector(directionX, directionY, directionZ);
}

Ray::~Ray()
{
    delete m_origin;
    delete m_direction;
}

Point* Ray::getOrigin() { return m_origin; }

Vector* Ray::getDirection() { return m_direction; }
