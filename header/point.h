#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point();
    Point(float x, float y, float z);
    float getX();
    float getY();
    float getZ();
    float setX(float x);
    float setY(float y);
    float setZ(float z);

private:
    float m_x;
    float m_y;
    float m_z;
};

#endif