#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "color.h"
#include "point.h"

/**
 * @brief Light Source.
 *
 * Represents a light source that has a specific color.
 */
class LightSource {
public:
    LightSource(Point position, Color color);
    Point getPosition();
    Color getColor();
    void setPosition(Point position);
    void setColor(Color color);
    void transform(Eigen::Matrix4d transMat);

private:
    Point m_position;
    Color m_color;
};

#endif