#include "../header/radiance.h"
#include "../header/constants.h"

Radiance::Radiance(float radianceRed, float radianceGreen, float radianceBlue)
{
    m_red = radianceRed;
    m_green = radianceGreen;
    m_blue = radianceBlue;
}

Radiance::Radiance(Color color)
{
    m_red = color.getRed() * MAXIMUM_IRRADIANCE;
    m_green = color.getGreen() * MAXIMUM_IRRADIANCE;
    m_blue = color.getBlue() * MAXIMUM_IRRADIANCE;
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

void Radiance::addRadiance(Radiance r)
{
    m_red += r.getRadianceRed();
    m_green += r.getRadianceGreen();
    m_blue += r.getRadianceBlue();
}

void Radiance::scaleRadiance(float scale)
{
    m_red *= scale;
    m_green *= scale;
    m_blue *= scale;
}

float Radiance::getIlluminance() { return 0.27 * m_red + 0.67 * m_green + 0.06 * m_blue; }