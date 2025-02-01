#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.h"
#include "point.h"

class Triangle : public Object {
public:
    Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
    ~Triangle();
    Ray* intersect(Ray* ray) override;
    void transform(Eigen::Matrix4d transMat) override;

private:
    Point* m_point0;
    Point* m_point1;
    Point* m_point2;
    Vector* m_e1;
    Vector* m_e2;
};

#endif