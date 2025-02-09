#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "point.h"
#include "radiance.h"

/**
 * @brief Light Source.
 *
 * Represents a light source that has a specific color.
 */
class LightSource {
public:
    LightSource(Point position, Radiance color);
    Point getPosition();
    Radiance getRadiance();
    void setPosition(Point position);
    void setRadiance(Radiance radiance);
    void transform(Eigen::Matrix4d transMat);

private:
    Point m_position;
    Radiance m_radiance;
};

#endif