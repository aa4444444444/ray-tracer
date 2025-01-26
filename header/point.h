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
    void setX(float x);
    void setY(float y);
    void setZ(float z);

private:
    float m_x;
    float m_y;
    float m_z;
};

#endif