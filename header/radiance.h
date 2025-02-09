#ifndef RADIANCE_H
#define RADIANCE_H

/**
 * @brief Radiance.
 *
 * Represents a Radiance value. Unlike color, Radiance is unbounded.
 */
class Radiance {
public:
    Radiance(float radianceRed, float radianceGreen, float radianceBlue);
    Radiance();
    float getRadianceRed();
    float getRadianceGreen();
    float getRadianceBlue();
    void setRadianceRed(float red);
    void setRadianceGreen(float green);
    void setRadianceBlue(float blue);
    void capRadiance();

private:
    float m_red;
    float m_green;
    float m_blue;
};

#endif