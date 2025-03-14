#ifndef POINT_H
#define POINT_H

#include <Eigen/Dense>

/**
 * @brief Represent a point in 3D space.
 */
class Point {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Point();
    Point(const Eigen::Vector3d& point);
    Point(float x, float y, float z);
    Point(Point* point);
    Eigen::Vector3d getPoint();
    void setPoint(const Eigen::Vector3d&);
    void transform(const Eigen::Matrix4d& transMat);
    void scale(float scaleAmount);
    float distance(Point* p);

private:
    Eigen::Vector3d m_point;
};

#endif