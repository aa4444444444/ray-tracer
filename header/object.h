#ifndef OBJECT_H
#define OBJECT_H

#include "color.h"
#include "intersection.h"
#include "ray.h"
#include <string>

/**
 * @brief Parent class for renderable objects.
 */
class Object {
public:
    virtual Intersection* intersect(Ray* ray) = 0;
    virtual void transform(Eigen::Matrix4d transMat) = 0;
    Color getColor() { return m_color; }
    void setColor(Color c)
    {
        m_color.setRed(c.getRed());
        m_color.setGreen(c.getGreen());
        m_color.setBlue(c.getBlue());
    }
    virtual ~Object() { };

protected:
    Color m_color;

private:
    std::string material;
};

#endif