#include "../header/triangle.h"

Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    m_point0 = new Point(x1, y1, z1);
    m_point1 = new Point(x2, y2, z2);
    m_point2 = new Point(x3, y3, z3);
    m_e1 = new Vector(m_point1->getPoint() - m_point0->getPoint());
    m_e2 = new Vector(m_point2->getPoint() - m_point0->getPoint());
}

Triangle::~Triangle()
{
    delete m_point0;
    delete m_point1;
    delete m_point2;
    delete m_e1;
    delete m_e2;
}

Ray* Triangle::intersect(Ray* ray)
{
    // DO NOT NORMALIZE THESE
    Vector* T = new Vector(ray->getOrigin()->getPoint() - m_point0->getPoint());
    Vector* P = new Vector(ray->getDirection()->cross(m_e2));
    Vector* Q = new Vector(T->cross(m_e1));

    float denominator = P->dot(m_e1);

    if (denominator == 0) {
        // No intersect
        return nullptr;
    } else {
        float u, v, w;
        w = (Q->dot(m_e2)) / denominator;
        u = (P->dot(T)) / denominator;
        v = (Q->dot(ray->getDirection())) / denominator;

        if (w < 0) {
            // Intersection behind ray origin, ignore
            return nullptr;
        } else if (u < 0 || v < 0 || (u + v) > 1) {
            // Intersection point outside triangle, ignore
            return nullptr;
        } else {
            Eigen::Vector3d intersectPoint
                = (1 - u - v) * m_point0->getPoint() + u * m_point1->getPoint() + v * m_point2->getPoint();
            Eigen::Vector3d normal = (m_e1->cross(m_e2))->getVector();
            return new Ray(intersectPoint(0), intersectPoint(1), intersectPoint(2), normal(0), normal(1), normal(2));
        }
    }
}

void Triangle::transform(Eigen::Matrix4d transMat)
{
    Eigen::Vector4d augmentedPoint0(m_point0->getPoint()(0), m_point0->getPoint()(1), m_point0->getPoint()(2), 1);
    Eigen::Vector4d transformedPoint0 = transMat * augmentedPoint0;
    Eigen::Vector3d newPoint0(transformedPoint0(0), transformedPoint0(1), transformedPoint0(2));
    m_point0->setPoint(newPoint0);

    Eigen::Vector4d augmentedPoint1(m_point1->getPoint()(0), m_point1->getPoint()(1), m_point1->getPoint()(2), 1);
    Eigen::Vector4d transformedPoint1 = transMat * augmentedPoint1;
    Eigen::Vector3d newPoint1(transformedPoint1(0), transformedPoint1(1), transformedPoint1(2));
    m_point1->setPoint(newPoint1);

    Eigen::Vector4d augmentedPoint2(m_point2->getPoint()(0), m_point2->getPoint()(1), m_point2->getPoint()(2), 1);
    Eigen::Vector4d transformedPoint2 = transMat * augmentedPoint2;
    Eigen::Vector3d newPoint2(transformedPoint2(0), transformedPoint2(1), transformedPoint2(2));
    m_point2->setPoint(newPoint2);

    Eigen::Vector3d newe1 = m_point1->getPoint() - m_point0->getPoint();
    m_e1->setVector(newe1);

    Eigen::Vector3d newe2 = m_point2->getPoint() - m_point0->getPoint();
    m_e2->setVector(newe2);
}