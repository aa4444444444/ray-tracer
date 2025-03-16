#include "../header/point.h"
#include <cmath>

Point::Point(float x, float y, float z) { m_point = Eigen::Vector3d(x, y, z); }

Point::Point() { m_point = Eigen::Vector3d(0, 0, 0); }

Point::Point(const Eigen::Vector3d& point) { m_point = point; }

Point::Point(Point* point) { m_point = point->getPoint(); }

Eigen::Vector3d Point::getPoint() { return m_point; }

void Point::setPoint(const Eigen::Vector3d& point) { m_point = point; }

void Point::transform(const Eigen::Matrix4d& transMat)
{
    Eigen::Vector4d augmented(m_point(0), m_point(1), m_point(2), 1.0f);
    Eigen::Vector4d result = transMat * augmented;
    m_point(0) = result(0) / result(3);
    m_point(1) = result(1) / result(3);
    m_point(2) = result(2) / result(3);
}

void Point::scale(float scaleAmount) { m_point = m_point * scaleAmount; }

void Point::translate(const Eigen::Vector3d& translate) { m_point = m_point + translate; }

float Point::distance(Point* p)
{
    return sqrt((m_point(0) - p->getPoint()(0)) * (m_point(0) - p->getPoint()(0))
        + (m_point(1) - p->getPoint()(1)) * (m_point(1) - p->getPoint()(1))
        + (m_point(2) - p->getPoint()(2)) * (m_point(2) - p->getPoint()(2)));
}