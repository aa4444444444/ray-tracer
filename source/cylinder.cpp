#include "../header/cylinder.h"
#include "../header/util.h"

Cylinder::Cylinder(Point basePoint, Point topPoint, double radius)
{
    m_base = basePoint;
    m_top = topPoint;
    m_radius = radius;
    setColor(Color(1.0, 0.0, 0.0));
    setSpecColor(Color(1.0, 1.0, 1.0));
}

// Based off of Graphics Gems III and IV
// https://www.realtimerendering.com/resources/GraphicsGems/
Intersection* Cylinder::intersect(Ray* ray)
{
    Point ro = ray->getOrigin();
    Vector rd = ray->getDirection();

    Point p1 = m_base;
    Point p2 = m_top;
    Vector ca = Vector(p2.getPoint() - p1.getPoint()); // Cylinder axis
    Vector oc = Vector(ro.getPoint() - p1.getPoint()); // Origin to base
    float height2 = ca.dot(&ca);
    float radius2 = m_radius * m_radius;

    float card = ca.dot(&rd);
    float caoc = ca.dot(&oc);

    Vector rd_proj = Vector(rd.getVector() - ca.getVector() * (card / height2));
    Vector oc_proj = Vector(oc.getVector() - ca.getVector() * (caoc / height2));

    float a = rd_proj.dot(&rd_proj);
    float b = 2.0f * rd_proj.dot(&oc_proj);
    float c = oc_proj.dot(&oc_proj) - radius2;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0f)
        return nullptr;

    float sqrt_disc = sqrt(discriminant);
    float t = (-b - sqrt_disc) / (2.0f * a);

    // Check side intersection
    if (t > 0.0f) {
        Point hit = Point(ro.getPoint() + rd.getVector() * t);
        Vector hitMinusP1 = Vector(hit.getPoint() - p1.getPoint());
        float y = ca.dot(&hitMinusP1);
        if (y >= 0.0f && y <= height2) {
            // Compute normal
            Vector proj = Vector(ca.getVector() * (y / height2));
            Vector normal = Vector(hit.getPoint() - p1.getPoint() - proj.getVector());
            normal.normalize();
            return new Intersection(hit, normal);
        }
    }

    // Cap intersections
    // Bottom cap (p1)
    float denom = ca.dot(&rd);
    if (fabs(denom) > 1e-6) {
        Vector p1MinusRo = Vector(p1.getPoint() - ro.getPoint());
        float t_cap = ca.dot(&p1MinusRo) / denom;
        if (t_cap > 0.0f) {
            Point hit = Point(ro.getPoint() + rd.getVector() * t_cap);
            Vector hitMinusP1 = Vector(hit.getPoint() - p1.getPoint());

            if (hitMinusP1.dot(&hitMinusP1) <= radius2) {
                Vector normal = Vector(-1.0 * ca.getVector());
                normal.normalize();
                return new Intersection(hit, normal);
            }
        }
    }

    // Top cap (p2)
    if (fabs(denom) > 1e-6) {
        Vector p2MinusRo = Vector(p2.getPoint() - ro.getPoint());
        float t_cap = ca.dot(&p2MinusRo) / denom;
        if (t_cap > 0.0f) {
            Point hit = Point(ro.getPoint() + rd.getVector() * t_cap);
            Vector hitMinusP2 = Vector(hit.getPoint() - p2.getPoint());
            if (hitMinusP2.dot(&hitMinusP2) <= radius2) {
                Vector normal = Vector(ca.getVector());
                normal.normalize();
                return new Intersection(hit, normal);
            }
        }
    }

    return nullptr;
}

void Cylinder::transform(const Eigen::Matrix4d& transMat)
{
    m_base.transform(transMat);
    m_top.transform(transMat);
}

// DOESN'T DO THE RIGHT THING AS OF RIGHT NOW
Eigen::Vector2d Cylinder::getTextureUV(Point intersectionPoint) { return Eigen::Vector2d(1.0f, 1.0f); }

AxisAlignedBoundingBox* Cylinder::getAxisAlignedBoundingBox()
{
    Eigen::Vector3d base = m_base.getPoint();
    Eigen::Vector3d top = m_top.getPoint();

    Eigen::Vector3d minPt = base.cwiseMin(top) - Eigen::Vector3d::Constant(m_radius);
    Eigen::Vector3d maxPt = base.cwiseMax(top) + Eigen::Vector3d::Constant(m_radius);

    return new AxisAlignedBoundingBox(static_cast<float>(minPt.x()), static_cast<float>(maxPt.x()),
        static_cast<float>(minPt.y()), static_cast<float>(maxPt.y()), static_cast<float>(minPt.z()),
        static_cast<float>(maxPt.z()), this);
}