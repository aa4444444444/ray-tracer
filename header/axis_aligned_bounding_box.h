#ifndef AXIS_ALIGNED_BOUNDING_BOX_H
#define AXIS_ALIGNED_BOUNDING_BOX_H

#include "ray.h"
#include <iostream>

class Object;
class Intersection;

struct AABBIntersection {
    AABBIntersection(float tStart, float tEnd, float t, bool intersects)
        : m_tStart(tStart)
        , m_tEnd(tEnd)
        , m_t(t)
        , m_intersects(intersects)
    {
    }
    float m_tStart, m_tEnd, m_t;
    bool m_intersects;
};

class AxisAlignedBoundingBox {
public:
    AxisAlignedBoundingBox();
    AxisAlignedBoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
    AxisAlignedBoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, Object* object);
    const float getXMin();
    const float getXMax();
    const float getYMin();
    const float getYMax();
    const float getZMin();
    const float getZMax();
    Object* getObject();
    void setXMin(float xMin);
    void setXMax(float xMax);
    void setYMin(float yMin);
    void setYMax(float yMax);
    void setZMin(float zMin);
    void setZMax(float zMax);
    void setObject(Object* object);
    float getVolume();
    float getSurfaceArea();
    float getMinInDirection(int direction);
    float getMaxInDirection(int direction);
    float getMidPoint(int direction);
    bool intersectionInsideBox(Intersection* intersection);
    AABBIntersection intersectRay(Ray* ray, float splitDistance, int splitPlane);
    bool intersect(AxisAlignedBoundingBox* aabb);
    void printAABB();

private:
    float m_xMin, m_xMax;
    float m_yMin, m_yMax;
    float m_zMin, m_zMax;
    Object* m_Object; // If AABB is associated with an object, by default nullptr
};

#endif