#include "../header/phong.h"

Phong::Phong(float k_a, float k_d, float k_s, float k_e)
{
    m_ka = k_a;
    m_kd = k_d;
    m_ks = k_s;
    m_ke = k_e;
}

Radiance Phong::illuminate(
    Color objectColor, Color specColor, Intersection* intersection, std::vector<LightSource*> lightSources)
{
    float ambientComponentRed = m_ka * objectColor.getRed() * BACKGROUND_RADIANCE_RED;
    float ambientComponentGreen = m_ka * objectColor.getGreen() * BACKGROUND_RADIANCE_GREEN;
    float ambientComponentBlue = m_ka * objectColor.getBlue() * BACKGROUND_RADIANCE_BLUE;

    return Radiance(ambientComponentRed, ambientComponentGreen, ambientComponentBlue);
}