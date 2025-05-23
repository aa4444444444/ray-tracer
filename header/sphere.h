#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"
#include "point.h"

/**
 * @brief A renderable sphere.
 *
 */
class Sphere : public Object {
public:
    Sphere(float centerX, float centerY, float centerZ, float radius);
    Point getCenter();
    float getRadius();
    void setRadius(float r);
    Intersection* intersect(Ray* ray) override;
    void transform(const Eigen::Matrix4d& transMat) override;
    Eigen::Vector2d getTextureUV(Point intersectionPoint) override;
    AxisAlignedBoundingBox* getAxisAlignedBoundingBox() override;

private:
    Point m_center;
    float m_radius;
};

#endif