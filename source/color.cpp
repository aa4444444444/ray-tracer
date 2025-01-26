#include "../header/color.h"

Color::Color(short red, short green, short blue)
{
    m_red = red;
    m_green = green;
    m_blue = blue;
};

short Color::getRed()
{
    return m_red;
}

short Color::getGreen()
{
    return m_green;
}

short Color::getBlue()
{
    return m_blue;
}

void Color::setRed(short red)
{
    m_red = red;
}

void Color::setGreen(short green)
{
    m_green = green;
}

void Color::setBlue(short blue)
{
    m_blue = blue;
}