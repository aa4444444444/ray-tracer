#include "../header/intersection.h"

Intersection::Intersection(Point intersection, Vector normal, Vector light, Vector reflection, Vector viewing)
{
    m_intersectionPoint = Point(intersection.getPoint());
    m_normal = Vector(normal.getVector());
    m_lightDirection = Vector(light.getVector());
    m_perfectMirrorReflectionDirection = Vector(reflection.getVector());
    m_viewingDirection = Vector(viewing.getVector());
}

Intersection::Intersection(Point intersection, Vector normal)
{
    m_intersectionPoint = Point(intersection.getPoint());
    m_normal = Vector(normal.getVector());
    m_lightDirection = Vector(0, 0, 0);
    m_perfectMirrorReflectionDirection = Vector(0, 0, 0);
    m_viewingDirection = Vector(0, 0, 0);
}

Intersection::Intersection()
{
    m_intersectionPoint = Point(0, 0, 0);
    m_normal = Vector(0, 0, 0);
    m_lightDirection = Vector(0, 0, 0);
    m_perfectMirrorReflectionDirection = Vector(0, 0, 0);
    m_viewingDirection = Vector(0, 0, 0);
}

void Intersection::normalizeAllVectors()
{
    m_normal.normalize();
    m_lightDirection.normalize();
    m_perfectMirrorReflectionDirection.normalize();
    m_viewingDirection.normalize();
}

Point Intersection::getIntersectionPoint() { return m_intersectionPoint; }

void Intersection::setIntersectionPoint(Point intersection) { m_intersectionPoint = Point(intersection.getPoint()); }

Vector Intersection::getNormal() { return m_normal; }

void Intersection::setNormal(Vector normal) { m_normal = Vector(normal.getVector()); }

Vector Intersection::getLightDirection() { return m_lightDirection; }

void Intersection::setLightDirection(Vector lightDirection) { m_lightDirection = Vector(lightDirection.getVector()); }

Vector Intersection::getPerfectMirrorReflectionDirection() { return m_perfectMirrorReflectionDirection; }

void Intersection::setPerfectMirrorReflectionDirection(Vector reflection)
{
    m_perfectMirrorReflectionDirection = Vector(reflection.getVector());
}

Vector Intersection::getViewingDirection() { return m_viewingDirection; }

void Intersection::setViewingDirection(Vector viewing) { m_viewingDirection = Vector(viewing.getVector()); }

std::vector<LightSource*> Intersection::getLightSources() { return m_lightSourceList; }
void Intersection::setLightSources(std::vector<LightSource*> lights) { m_lightSourceList = lights; }

Object* Intersection::getObject() { return m_object; }
void Intersection::setObject(Object* object) { m_object = object; }