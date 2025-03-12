#include "../header/axis_aligned_bounding_box.h"
#include "../header/intersection.h"
#include "../header/object.h"

AxisAlignedBoundingBox::AxisAlignedBoundingBox()
{
    m_xMin = 0.0f, m_xMax = 0.0f, m_yMin = 0.0f, m_yMax = 0.0f, m_zMin = 0.0f, m_zMax = 0.0f, m_Object = nullptr;
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
    m_xMin = xMin, m_xMax = xMax, m_yMin = yMin, m_yMax = yMax, m_zMin = zMin, m_zMax = zMax, m_Object = nullptr;
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(
    float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, Object* object)
{
    m_xMin = xMin, m_xMax = xMax, m_yMin = yMin, m_yMax = yMax, m_zMin = zMin, m_zMax = zMax, m_Object = object;
}
const float AxisAlignedBoundingBox::getXMin() { return m_xMin; }
const float AxisAlignedBoundingBox::getXMax() { return m_xMax; }
const float AxisAlignedBoundingBox::getYMin() { return m_yMin; }
const float AxisAlignedBoundingBox::getYMax() { return m_yMax; }
const float AxisAlignedBoundingBox::getZMin() { return m_zMin; }
const float AxisAlignedBoundingBox::getZMax() { return m_zMax; }
Object* AxisAlignedBoundingBox::getObject() { return m_Object; }
void AxisAlignedBoundingBox::setXMin(float xMin) { m_xMin = xMin; }
void AxisAlignedBoundingBox::setXMax(float xMax) { m_xMax = xMax; }
void AxisAlignedBoundingBox::setYMin(float yMin) { m_yMin = yMin; }
void AxisAlignedBoundingBox::setYMax(float yMax) { m_yMax = yMax; }
void AxisAlignedBoundingBox::setZMin(float zMin) { m_zMin = zMin; }
void AxisAlignedBoundingBox::setZMax(float zMax) { m_zMax = zMax; }
void AxisAlignedBoundingBox::setObject(Object* object) { m_Object = object; }
float AxisAlignedBoundingBox::getVolume() { return (m_xMax - m_xMin) * (m_yMax - m_yMin) * (m_zMax - m_zMin); }
bool AxisAlignedBoundingBox::intersect(AxisAlignedBoundingBox* aabb)
{
    if (m_xMin > aabb->getXMax() || aabb->getXMin() > m_xMax) {
        return false;
    }

    if (m_yMin > aabb->getYMax() || aabb->getYMin() > m_yMax) {
        return false;
    }

    if (m_zMin > aabb->getZMax() || aabb->getZMin() > m_zMax) {
        return false;
    }

    return true;
}

bool AxisAlignedBoundingBox::intersectionInsideBox(Intersection* intersection)
{
    Point p = intersection->getIntersectionPoint();
    float px = p.getPoint()(0);
    float py = p.getPoint()(1);
    float pz = p.getPoint()(2);

    if (m_xMin > px || m_xMax < px) {
        return false;
    }

    if (m_yMin > py || m_yMax < py) {
        return false;
    }

    if (m_zMin > pz || m_zMax < pz) {
        return false;
    }

    return true;
}

AABBIntersection AxisAlignedBoundingBox::intersectRay(Ray* ray, float splitDistance, int splitPlane)
{
    Eigen::Vector3d direction = ray->getDirection().getVector();
    Eigen::Vector3d origin = ray->getOrigin().getPoint();

    // For each dimension, check if no intersection
    if (direction(0) == 0 && (origin(0) < m_xMin || origin(0) > m_xMax)) {
        return AABBIntersection(0, 0, 0, false);
    }
    if (direction(1) == 0 && (origin(1) < m_yMin || origin(1) > m_yMax)) {
        return AABBIntersection(0, 0, 0, false);
    }
    if (direction(2) == 0 && (origin(2) < m_zMin || origin(2) > m_zMax)) {
        return AABBIntersection(0, 0, 0, false);
    }

    // r.dir is unit direction vector of ray
    // Pre-compute division for speed
    float directionXReciprocal = 1.0f / direction(0);
    float directionYReciprocal = 1.0f / direction(1);
    float directionZReciprocal = 1.0f / direction(2);

    float t;

    // Calculate t (intersection of split plane)
    if (splitPlane == 0) {
        // Plane on X-Axis
        t = (splitDistance - origin(0)) * directionXReciprocal;
    } else if (splitPlane == 1) {
        // Plane on Y-Axis
        t = (splitDistance - origin(1)) * directionYReciprocal;
    } else {
        // Plane on Z-Axis
        t = (splitDistance - origin(2)) * directionZReciprocal;
    }

    // For each dimension calculate intersection distances t1 and t2
    float t1x = (m_xMin - origin(0)) * directionXReciprocal;
    float t2x = (m_xMax - origin(0)) * directionXReciprocal;

    // If t1 > t2, swap
    if (t1x > t2x) {
        float temp = t1x;
        t1x = t2x;
        t2x = temp;
    }

    float tStart = t1x;
    float tEnd = t2x;

    float t1y = (m_yMin - origin(1)) * directionYReciprocal;
    float t2y = (m_yMax - origin(1)) * directionYReciprocal;

    if (t1y > t2y) {
        float temp = t1y;
        t1y = t2y;
        t2y = temp;
    }

    // Intersect with interval
    if (t1y > tStart) {
        tStart = t1y;
    }

    if (t2y < tEnd) {
        tEnd = t2y;
    }

    float t1z = (m_zMin - origin(2)) * directionZReciprocal;
    float t2z = (m_zMax - origin(2)) * directionZReciprocal;

    if (t1z > t2z) {
        float temp = t1z;
        t1z = t2z;
        t2z = temp;
    }

    // Intersect with interval
    if (t1z > tStart) {
        tStart = t1z;
    }

    if (t2z < tEnd) {
        tEnd = t2z;
    }

    // Determine if intersection or no intersection

    // Box is missed
    if (tStart > tEnd) {
        return AABBIntersection(tStart, tEnd, t, false);
    }

    // Box is behind
    if (tEnd < 0.0f) {
        return AABBIntersection(tStart, tEnd, t, false);
    }

    // Closest intersection at tStart
    if (tStart > 0.0f) {
        return AABBIntersection(tStart, tEnd, t, true);
    } else {
        // Closest intersection at tEnd
        return AABBIntersection(tStart, tEnd, t, true);
    }
}

void AxisAlignedBoundingBox::printAABB()
{
    std::cout << "AABB: X(" << m_xMin << ", " << m_xMax << "), Y(" << m_yMin << ", " << m_yMax << "), Z(" << m_zMin
              << ", " << m_zMax << ")" << std::endl;
}