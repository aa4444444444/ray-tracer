#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"
#include "point.h"

class Sphere : public Object {
public:
    Sphere(float centerX, float centerY, float centerZ, float radius);
    Point getCenter();
    float getRadius();
    void setRadius(float r);
    Ray* intersect(Ray* ray) override;
    void transform(Eigen::Matrix4d transMat) override;

private:
    Point m_center;
    float m_radius;
};

#endif