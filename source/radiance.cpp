#include "../header/radiance.h"
#include "../header/constants.h"

Radiance::Radiance(float radianceRed, float radianceGreen, float radianceBlue)
{
    m_red = radianceRed;
    m_green = radianceGreen;
    m_blue = radianceBlue;
}

Radiance::Radiance()
{
    m_red = 0.0;
    m_green = 0.0;
    m_blue = 0.0;
}

float Radiance::getRadianceRed() { return m_red; }
float Radiance::getRadianceGreen() { return m_green; }
float Radiance::getRadianceBlue() { return m_blue; }
void Radiance::setRadianceRed(float red) { m_red = red; }
void Radiance::setRadianceGreen(float green) { m_green = green; }
void Radiance::setRadianceBlue(float blue) { m_blue = blue; }
void Radiance::capRadiance()
{
    if (m_red > MAXIMUM_IRRADIANCE) {
        m_red = MAXIMUM_IRRADIANCE;
    }

    if (m_green > MAXIMUM_IRRADIANCE) {
        m_green = MAXIMUM_IRRADIANCE;
    }

    if (m_blue > MAXIMUM_IRRADIANCE) {
        m_blue = MAXIMUM_IRRADIANCE;
    }
}