#ifndef POINT_H
#define POINT_H

#include <Eigen/Dense>

class Point {
public:
    Point();
    Point(float x, float y, float z);
    Eigen::Vector3d getPoint();
    void setPoint(Eigen::Vector3d);
    void transform(Eigen::Matrix4d transMat);
    float distance(Point p);

private:
    Eigen::Vector3d m_point;
};

#endif