#include "../header/color.h"

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

float Color::getRed() { return m_red; }

float Color::getGreen() { return m_green; }

float Color::getBlue() { return m_blue; }

void Color::setRed(float red) { m_red = red; }

void Color::setGreen(float green) { m_green = green; }

void Color::setBlue(float blue) { m_blue = blue; }

int Color::getRed255() { return static_cast<int>(m_red * 255); }

int Color::getGreen255() { return static_cast<int>(m_green * 255); }

int Color::getBlue255() { return static_cast<int>(m_blue * 255); }