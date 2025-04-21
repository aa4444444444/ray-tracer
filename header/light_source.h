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
    bool getIsInShadow();
    void setIsInShadow(bool isInShadow);

private:
    Point m_position;
    Radiance m_radiance;

    // Since a list of light sources is passed to illumination models, it's easier
    // to determine whether a point is in shadow from a certain light source by
    // associating this boolean with the light source and not the intersection
    bool m_isInShadow;
};

#endif