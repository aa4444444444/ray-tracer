#include "../header/cone.h"
#include "../header/util.h"

Cone::Cone(Point basePoint, Point tipPoint, double radius)
{
    m_base = basePoint;
    m_tip = tipPoint;
    m_radius = radius;
    setColor(Color(1.0, 0.0, 0.0));
    setSpecColor(Color(1.0, 1.0, 1.0));
}

Intersection* Cone::intersect(Ray* ray)
{
    Vector d = ray->getDirection();
    Point o = ray->getOrigin();

    Vector ca = Vector(m_base.getPoint() - m_tip.getPoint()); // cone axis
    Vector oc = Vector(o.getPoint() - m_tip.getPoint()); // origin to tip
    float height = ca.magnitude();
    ca.normalize();

    float cos2 = (height * height) / (height * height + m_radius * m_radius);

    float dv = d.dot(&ca);
    float ocv = oc.dot(&ca);

    Vector d_proj = Vector(d.getVector() - ca.getVector() * dv);
    Vector oc_proj = Vector(oc.getVector() - ca.getVector() * ocv);

    float A = d_proj.dot(&d_proj) - cos2 * dv * dv;
    float B = 2 * (d_proj.dot(&oc_proj) - cos2 * dv * ocv);
    float C = oc_proj.dot(&oc_proj) - cos2 * ocv * ocv;

    float disc = B * B - 4 * A * C;
    if (disc < 0)
        return nullptr;

    float sqrt_disc = sqrt(disc);
    float t0 = (-B - sqrt_disc) / (2 * A);
    float t1 = (-B + sqrt_disc) / (2 * A);

    if (t0 > t1)
        std::swap(t0, t1);

    for (float t : { t0, t1 }) {
        if (t < 1e-4)
            continue;

        Point hit = Point(o.getPoint() + d.getVector() * t);
        Vector v = Vector(hit.getPoint() - m_tip.getPoint());
        float h = v.dot(&ca);

        if (h >= 0 && h <= height) {
            Vector axis_to_point = Vector(ca.getVector() * h);
            Vector center = Vector(m_tip.getPoint() + axis_to_point.getVector());

            // Now the normal is the vector from the axis center to hit, scaled properly
            Vector radial = Vector(hit.getPoint() - center.getVector());
            float scale = m_radius / height;
            Vector normal
                = Vector(radial.getVector() - ca.getVector() * scale * radial.dot(&ca)); // remove component along axis

            normal.normalize();
            return new Intersection(hit, normal);
        }
    }

    // Optional: base cap
    float denom = ca.dot(&d);
    if (fabs(denom) > 1e-6) {
        Vector baseMinusO = Vector((m_base.getPoint() - o.getPoint()));
        float t_cap = ca.dot(&baseMinusO) / denom;
        if (t_cap > 1e-4) {
            Point hit = Point(o.getPoint() + d.getVector() * t_cap);
            Vector hitToBase = Vector(hit.getPoint() - m_base.getPoint());
            float hitTobaseLenSquared = hitToBase.dot(&hitToBase);
            if (hitTobaseLenSquared <= m_radius * m_radius) {
                Vector normal = Vector(-1.0 * ca.getVector());
                normal.normalize();
                return new Intersection(hit, normal);
            }
        }
    }

    return nullptr;
}

void Cone::transform(const Eigen::Matrix4d& transMat)
{
    m_base.transform(transMat);
    m_tip.transform(transMat);
}

// DOESN'T DO THE RIGHT THING AS OF RIGHT NOW
Eigen::Vector2d Cone::getTextureUV(Point intersectionPoint) { return Eigen::Vector2d(1.0f, 1.0f); }

AxisAlignedBoundingBox* Cone::getAxisAlignedBoundingBox()
{
    Eigen::Vector3d base = m_base.getPoint();
    Eigen::Vector3d top = m_tip.getPoint();

    Eigen::Vector3d minPt = base.cwiseMin(top) - Eigen::Vector3d::Constant(m_radius);
    Eigen::Vector3d maxPt = base.cwiseMax(top) + Eigen::Vector3d::Constant(m_radius);

    return new AxisAlignedBoundingBox(static_cast<float>(minPt.x()), static_cast<float>(maxPt.x()),
        static_cast<float>(minPt.y()), static_cast<float>(maxPt.y()), static_cast<float>(minPt.z()),
        static_cast<float>(maxPt.z()), this);
}