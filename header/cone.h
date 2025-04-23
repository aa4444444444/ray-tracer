#ifndef CONE_H
#define CONE_H

#include "object.h"
#include <Eigen/Dense>

class Cone : public Object {
public:
    Cone(Point basePoint, Point tipPoint, double radius);

    Intersection* intersect(Ray* ray) override;
    void transform(const Eigen::Matrix4d& transMat) override;
    Eigen::Vector2d getTextureUV(Point intersectionPoint) override;
    AxisAlignedBoundingBox* getAxisAlignedBoundingBox() override;

private:
    Point m_base; // Center of the base
    Point m_tip; // Center of the top
    double m_radius;
};

#endif
