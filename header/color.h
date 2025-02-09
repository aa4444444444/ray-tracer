#ifndef COLOR_H
#define COLOR_H

#include "radiance.h"

/**
 * @brief Color.
 *
 * Represents an RGB color.
 */
class Color {
public:
    Color(float red, float green, float blue);
    Color();
    Color(Radiance r);
    float getRed();
    float getGreen();
    float getBlue();
    void setRed(float red);
    void setGreen(float green);
    void setBlue(float blue);
    int getRed255();
    int getGreen255();
    int getBlue255();

private:
    float m_red;
    float m_green;
    float m_blue;
};

#endif