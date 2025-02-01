#include "../header/sphere.h"
#include <iostream>

Sphere::Sphere(float centerX, float centerY, float centerZ, float radius)
    : Object()
{
    m_center = Point(centerX, centerY, centerZ);
    m_radius = radius;
}

float getDiscriminant(float b, float c) { return b * b - 4 * c; }

Ray* Sphere::intersect(Ray* ray)
{
    Point rayOrigin = ray->getOrigin();
    float x_o = rayOrigin.getPoint()(0);
    float y_o = rayOrigin.getPoint()(1);
    float z_o = rayOrigin.getPoint()(2);
    Vector rayDirection = ray->getDirection();
    float d_x = rayDirection.getVector()(0);
    float d_y = rayDirection.getVector()(1);
    float d_z = rayDirection.getVector()(2);

    float x_c = m_center.getPoint()(0);
    float y_c = m_center.getPoint()(1);
    float z_c = m_center.getPoint()(2);

    float b = 2 * (d_x * (x_o - x_c) + d_y * (y_o - y_c) + d_z * (z_o - z_c));
    float c = (x_o - x_c) * (x_o - x_c) + (y_o - y_c) * (y_o - y_c) + (z_o - z_c) * (z_o - z_c)
        - this->getRadius() * this->getRadius();

    float discriminant = getDiscriminant(b, c);
    float omega;

    // If discriminant < 0, no real root (no real intersection)
    // If discriminant = 0, one root (ray intersects at sphere surface)
    // If discriminant > 0, two roots (ray goes through sphere) - use least positive root
    if (discriminant < 0) {
        return nullptr;
    } else if (discriminant == 0) {
        omega = -1 * b / 2.0;
    } else {
        float root_1 = (-1 * b + discriminant) / 2.0;
        float root_2 = (-1 * b - discriminant) / 2.0;
        if (root_1 < 0) {
            omega = root_2;
        } else if (root_2 < 0) {
            omega = root_1;
        } else {
            omega = std::min(root_1, root_2);
        }
    }

    float x_i = (x_o + d_x * omega);
    float y_i = (y_o + d_y * omega);
    float z_i = (z_o + d_z * omega);

    return new Ray(x_i, y_i, z_i, (x_i - x_c), (y_i - y_c), (z_i - z_c));
}

void Sphere::transform(Eigen::Matrix4d transMat)
{

    Eigen::Vector4d augmented(m_center.getPoint()(0), m_center.getPoint()(1), m_center.getPoint()(2), 1);
    Eigen::Vector4d transformed = transMat * augmented;
    Eigen::Vector3d newCenter(transformed(0), transformed(1), transformed(2));
    m_center.setPoint(newCenter);
}

Point Sphere::getCenter() { return m_center; }
float Sphere::getRadius() { return m_radius; }
void Sphere::setRadius(float r) { m_radius = r; }