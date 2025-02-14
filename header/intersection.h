#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "light_source.h"
#include "point.h"
#include "vector.h"
#include <vector>

class Object;

/**
 * @brief Represents an intersection in 3D space.
 */
class Intersection {
public:
    Intersection(Point intersection, Vector normal, Vector light, Vector reflection, Vector viewing);
    Intersection(Point intersection, Vector normal);
    Intersection();
    Point getIntersectionPoint();
    void setIntersectionPoint(Point intersection);
    Vector getNormal();
    void setNormal(Vector normal);
    Vector getLightDirection();
    void setLightDirection(Vector lightDirection);
    Vector getPerfectMirrorReflectionDirection();
    void setPerfectMirrorReflectionDirection(Vector reflection);
    Vector getViewingDirection();
    void setViewingDirection(Vector viewing);
    std::vector<LightSource*> getLightSources();
    void setLightSources(std::vector<LightSource*> lights);
    void normalizeAllVectors();
    Object* getObject();
    void setObject(Object* object);

private:
    Point m_intersectionPoint;
    Vector m_normal;
    Vector m_lightDirection;
    Vector m_perfectMirrorReflectionDirection;
    Vector m_viewingDirection;
    Object* m_object;
    std::vector<LightSource*> m_lightSourceList;
};

#endif