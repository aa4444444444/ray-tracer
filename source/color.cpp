#include "../header/color.h"
#include "../header/constants.h"
#include "../header/radiance.h"
#include <algorithm>

Color::Color(float red, float green, float blue)
{
    m_red = red;
    m_green = green;
    m_blue = blue;
}

Color::Color()
{
    m_red = 0;
    m_green = 0;
    m_blue = 0;
}

Color::Color(Radiance r)
{
    m_red = r.getRadianceRed() / MAXIMUM_IRRADIANCE;
    m_green = r.getRadianceGreen() / MAXIMUM_IRRADIANCE;
    m_blue = r.getRadianceBlue() / MAXIMUM_IRRADIANCE;
}

float Color::getRed() { return m_red; }

float Color::getGreen() { return m_green; }

float Color::getBlue() { return m_blue; }

void Color::setRed(float red) { m_red = red; }

void Color::setGreen(float green) { m_green = green; }

void Color::setBlue(float blue) { m_blue = blue; }

int Color::getRed255() { return static_cast<int>(m_red * 255); }

int Color::getGreen255() { return static_cast<int>(m_green * 255); }

int Color::getBlue255() { return static_cast<int>(m_blue * 255); }

Color Color::operator*(float scalar) const
{
    return Color(std::min(1.0f, std::max(0.0f, m_red * scalar)), std::min(1.0f, std::max(0.0f, m_green * scalar)),
        std::min(1.0f, std::max(0.0f, m_blue * scalar)));
}

Color& Color::operator*=(float scalar)
{
    m_red = std::min(1.0f, std::max(0.0f, m_red * scalar));
    m_green = std::min(1.0f, std::max(0.0f, m_green * scalar));
    m_blue = std::min(1.0f, std::max(0.0f, m_blue * scalar));
    return *this;
}