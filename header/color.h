#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
    Color(short red, short green, short blue);
    short getRed();
    short getGreen();
    short getBlue();
    void setRed(short red);
    void setGreen(short green);
    void setBlue(short blue);

private:
    short m_red;
    short m_green;
    short m_blue;
};

#endif