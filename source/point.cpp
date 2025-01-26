#include "../header/point.h"

Point::Point(float x, float y, float z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

Point::Point()
{
    m_x = 0.0;
    m_y = 0.0;
    m_z = 0.0;
}

float Point::getX()
{
    return m_x;
}

float Point::getY()
{
    return m_y;
}
float Point::getZ()
{
    return m_z;
}
void Point::setX(float x)
{
    m_x = x;
}
void Point::setY(float y)
{
    m_y = y;
}
void Point::setZ(float z)
{
    m_z = z;
}