#include "../header/vector.h"
#include <cmath>

Vector::Vector(float x, float y, float z)
{
    m_x = x;
    m_y = y;
    m_z = z;

    // Automatically normalize all vectors when created
    normalize();
}

float Vector::getX() { return m_x; }

float Vector::getY() { return m_y; }

float Vector::getZ() { return m_z; }

void Vector::setX(float x) { m_x = x; }

void Vector::setY(float y) { m_y = y; }

void Vector::setZ(float z) { m_z = z; }

void Vector::normalize()
{
    float mag = magnitude();
    if (mag != 0) {
        m_x = m_x / mag;
        m_y = m_y / mag;
        m_z = m_z / mag;
    }
}

float Vector::magnitude() { return sqrt(m_x * m_x + m_y * m_y + m_z * m_z); }

Vector Vector::cross(Vector v)
{
    return Vector(
        (m_y * v.getZ() - m_z * v.getY()), -1 * (m_x * v.getZ() - m_z * v.getX()), (m_x * v.getY() - m_y * v.getX()));
}

float Vector::dot(Vector v) { return m_x * v.getX() + m_y * v.getY() + m_z * v.getZ(); }