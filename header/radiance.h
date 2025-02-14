#ifndef RADIANCE_H
#define RADIANCE_H

#include "color.h"

/**
 * @brief Radiance.
 *
 * Represents a Radiance value. Unlike color, Radiance is unbounded.
 */
class Radiance {
public:
    Radiance(Color color);
    Radiance(float radianceRed, float radianceGreen, float radianceBlue);
    Radiance();
    float getRadianceRed();
    float getRadianceGreen();
    float getRadianceBlue();
    void setRadianceRed(float red);
    void setRadianceGreen(float green);
    void setRadianceBlue(float blue);
    void capRadiance();
    void addRadiance(Radiance r);
    void scaleRadiance(float scale);

private:
    float m_red;
    float m_green;
    float m_blue;
};

#endif