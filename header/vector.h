#ifndef VECTOR_H
#define VECTOR_H

class Vector {
public:
    Vector(float x, float y, float z);
    float getX();
    float getY();
    float getZ();
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    void normalize();
    float magnitude();
    Vector cross(Vector v);
    float dot(Vector v);

private:
    float m_x;
    float m_y;
    float m_z;
};

#endif