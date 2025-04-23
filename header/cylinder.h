#ifndef CYLINDER_H
#define CYLINDER_H

#include "object.h"
#include <Eigen/Dense>

class Cylinder : public Object {
public:
    Cylinder(Point basePoint, Point topPoint, double radius);

    Intersection* intersect(Ray* ray) override;
    void transform(const Eigen::Matrix4d& transMat) override;
    Eigen::Vector2d getTextureUV(Point intersectionPoint) override;
    AxisAlignedBoundingBox* getAxisAlignedBoundingBox() override;

private:
    Point m_base; // Center of the base
    Point m_top; // Center of the top
    double m_radius;
};

#endif