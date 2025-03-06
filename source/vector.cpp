#include "../header/vector.h"
#include <cmath>

Vector::Vector(const Eigen::Vector3d& vec) { m_vector = vec; }

Vector::Vector(float x, float y, float z) { m_vector = Eigen::Vector3d(x, y, z); }

Vector::Vector(Vector* vector) { m_vector = vector->getVector(); }

Vector::Vector() { m_vector = Eigen::Vector3d(0, 0, 0); }

Eigen::Vector3d Vector::getVector() { return m_vector; }

void Vector::setVector(const Eigen::Vector3d& vec) { m_vector = vec; }

void Vector::normalize()
{
    float mag = magnitude();
    if (mag != 0) {
        m_vector.normalize();
    }
}

float Vector::magnitude() { return m_vector.norm(); }

void Vector::scale(float scale) { m_vector = m_vector * scale; }

Vector Vector::cross(Vector* v) { return Vector(m_vector.cross(v->getVector())); }

float Vector::dot(Vector* v) { return m_vector.dot(v->getVector()); }

void Vector::transform(const Eigen::Matrix4d& transMat)
{
    Eigen::Vector4d augmented(m_vector(0), m_vector(1), m_vector(2), 1.0f);
    Eigen::Vector4d result = transMat * augmented;
    m_vector(0) = result(0) / result(3);
    m_vector(1) = result(1) / result(3);
    m_vector(2) = result(2) / result(3);
}