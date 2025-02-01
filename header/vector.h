#ifndef VECTOR_H
#define VECTOR_H

#include <Eigen/Dense>

/**
 * @brief Represents a vector in 3D space.
 *
 */
class Vector {
public:
    Vector(Eigen::Vector3d vec);
    Vector(float x, float y, float z);
    Vector(Vector* vector);
    Vector();
    Eigen::Vector3d getVector();
    void setVector(Eigen::Vector3d vec);
    void normalize();
    float magnitude();
    Vector cross(Vector* v);
    float dot(Vector* v);
    void scale(float scale);
    void transform(Eigen::Matrix4d transMat);

private:
    Eigen::Vector3d m_vector;
};

#endif