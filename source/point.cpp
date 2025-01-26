#include "../header/point.h"

Point::Point(float x, float y, float z) { m_point = Eigen::Vector3d(x, y, z); }

Point::Point() { m_point = Eigen::Vector3d(0, 0, 0); }

Eigen::Vector3d Point::getPoint() { return m_point; }

void Point::setPoint(Eigen::Vector3d point) { m_point = point; }

void Point::transform(Eigen::Matrix4d transMat)
{
    Eigen::Vector4d augmented(m_point(0), m_point(1), m_point(2), 1);
    Eigen::Vector4d result = transMat * augmented;
    m_point(0) = result(0) / result(3);
    m_point(1) = result(1) / result(3);
    m_point(2) = result(2) / result(3);
}