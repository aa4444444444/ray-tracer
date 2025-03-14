#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.h"
#include "point.h"

/**
 * @brief A renderable triangle.
 *
 */
class Triangle : public Object {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
    void setTexturePoints(const Eigen::Vector2d& texturePoint1, const Eigen::Vector2d& texturePoint2,
        const Eigen::Vector2d& texturePoint3);
    Intersection* intersect(Ray* ray) override;
    void scale(float scaleAmount);
    void transform(Eigen::Matrix4d transMat) override;
    Eigen::Vector2d getTextureUV(Point intersectionPoint) override;
    AxisAlignedBoundingBox* getAxisAlignedBoundingBox() override;

private:
    // Points subject to transformations
    Point m_point0;
    Point m_point1;
    Point m_point2;
    // Original points in object coordinates
    Point m_objectPoint0;
    Point m_objectPoint1;
    Point m_objectPoint2;
    // Texture Points
    Eigen::Vector2d m_texturePoint0;
    Eigen::Vector2d m_texturePoint1;
    Eigen::Vector2d m_texturePoint2;
    // Barycentric vectors subject to transformations
    Vector m_e1;
    Vector m_e2;
};

#endif