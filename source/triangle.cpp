#include "../header/triangle.h"

Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    m_point0 = Point(x1, y1, z1);
    m_point1 = Point(x2, y2, z2);
    m_point2 = Point(x3, y3, z3);

    m_objectPoint0 = Point(x1, y1, z1);
    m_objectPoint1 = Point(x2, y2, z2);
    m_objectPoint2 = Point(x3, y3, z3);

    // For barycentric coordinates
    m_e1 = Vector(m_point1.getPoint() - m_point0.getPoint());
    m_e2 = Vector(m_point2.getPoint() - m_point0.getPoint());

    // Default texture points
    m_texturePoint0 = Eigen::Vector2d(0.0f, 0.0f);
    m_texturePoint1 = Eigen::Vector2d(1.0f, 0.0f);
    m_texturePoint2 = Eigen::Vector2d(0.0f, 1.0f);
}

void Triangle::setTexturePoints(
    const Eigen::Vector2d& texturePoint1, const Eigen::Vector2d& texturePoint2, const Eigen::Vector2d& texturePoint3)
{
    m_texturePoint0 = texturePoint1;
    m_texturePoint1 = texturePoint2;
    m_texturePoint2 = texturePoint3;
}

Eigen::Vector2d Triangle::getTextureUV(Point intersectionPoint)
{
    Vector e1 = Vector(m_objectPoint1.getPoint() - m_objectPoint0.getPoint());
    Vector e2 = Vector(m_objectPoint2.getPoint() - m_objectPoint0.getPoint());
    Vector e3 = Vector(intersectionPoint.getPoint() - m_objectPoint0.getPoint());
    float d00 = e1.dot(&e1);
    float d01 = e1.dot(&e2);
    float d11 = e2.dot(&e2);
    float d20 = e3.dot(&e1);
    float d21 = e3.dot(&e2);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    return u * m_texturePoint0 + v * m_texturePoint1 + w * m_texturePoint2;
}

Intersection* Triangle::intersect(Ray* ray)
{
    Point rayOrigin = ray->getOrigin();
    Vector rayDirection = ray->getDirection();

    // DO NOT NORMALIZE THESE
    Vector T(rayOrigin.getPoint() - m_point0.getPoint());
    Vector P(rayDirection.cross(&m_e2));
    Vector Q(T.cross(&m_e1));

    float denominator = P.dot(&m_e1);

    if (denominator == 0) {
        // No intersect
        return nullptr;
    } else {
        float u, v, w;
        w = (Q.dot(&m_e2)) / denominator;
        u = (P.dot(&T)) / denominator;
        v = (Q.dot(&rayDirection)) / denominator;

        if (w < 0) {
            // Intersection behind ray origin, ignore
            return nullptr;
        } else if (u < 0 || v < 0 || (u + v) > 1) {
            // Intersection point outside triangle, ignore
            return nullptr;
        } else {
            Eigen::Vector3d intersectPoint
                = (1 - u - v) * m_point0.getPoint() + u * m_point1.getPoint() + v * m_point2.getPoint();
            Eigen::Vector3d normal = (m_e1.cross(&m_e2)).getVector();
            Vector normalVec = Vector(normal);
            normalVec.normalize();
            return new Intersection(Point(intersectPoint(0), intersectPoint(1), intersectPoint(2)), normalVec);
        }
    }
}

void Triangle::scale(float scaleAmount)
{
    m_point0.scale(scaleAmount);
    m_point1.scale(scaleAmount);
    m_point2.scale(scaleAmount);

    m_e1 = Vector(m_point1.getPoint() - m_point0.getPoint());
    m_e2 = Vector(m_point2.getPoint() - m_point0.getPoint());
}

void Triangle::translate(const Eigen::Vector3d& translate)
{
    m_point0.translate(translate);
    m_point1.translate(translate);
    m_point2.translate(translate);
}

void Triangle::transform(const Eigen::Matrix4d& transMat)
{
    Eigen::Vector4d augmentedPoint0(m_point0.getPoint()(0), m_point0.getPoint()(1), m_point0.getPoint()(2), 1);
    Eigen::Vector4d transformedPoint0 = transMat * augmentedPoint0;
    Eigen::Vector3d newPoint0(transformedPoint0(0), transformedPoint0(1), transformedPoint0(2));
    m_point0.setPoint(newPoint0);

    Eigen::Vector4d augmentedPoint1(m_point1.getPoint()(0), m_point1.getPoint()(1), m_point1.getPoint()(2), 1);
    Eigen::Vector4d transformedPoint1 = transMat * augmentedPoint1;
    Eigen::Vector3d newPoint1(transformedPoint1(0), transformedPoint1(1), transformedPoint1(2));
    m_point1.setPoint(newPoint1);

    Eigen::Vector4d augmentedPoint2(m_point2.getPoint()(0), m_point2.getPoint()(1), m_point2.getPoint()(2), 1);
    Eigen::Vector4d transformedPoint2 = transMat * augmentedPoint2;
    Eigen::Vector3d newPoint2(transformedPoint2(0), transformedPoint2(1), transformedPoint2(2));
    m_point2.setPoint(newPoint2);

    Eigen::Vector3d newe1 = m_point1.getPoint() - m_point0.getPoint();
    m_e1.setVector(newe1);

    Eigen::Vector3d newe2 = m_point2.getPoint() - m_point0.getPoint();
    m_e2.setVector(newe2);
}

AxisAlignedBoundingBox* Triangle::getAxisAlignedBoundingBox()
{
    float xMin = std::min(std::min(m_point0.getPoint()(0), m_point1.getPoint()(0)), m_point2.getPoint()(0));
    float xMax = std::max(std::max(m_point0.getPoint()(0), m_point1.getPoint()(0)), m_point2.getPoint()(0));
    float yMin = std::min(std::min(m_point0.getPoint()(1), m_point1.getPoint()(1)), m_point2.getPoint()(1));
    float yMax = std::max(std::max(m_point0.getPoint()(1), m_point1.getPoint()(1)), m_point2.getPoint()(1));
    float zMin = std::min(std::min(m_point0.getPoint()(2), m_point1.getPoint()(2)), m_point2.getPoint()(2));
    float zMax = std::max(std::max(m_point0.getPoint()(2), m_point1.getPoint()(2)), m_point2.getPoint()(2));

    return new AxisAlignedBoundingBox(xMin, xMax, yMin, yMax, zMin, zMax, this);
}