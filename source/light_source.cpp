#include "../header/light_source.h"

LightSource::LightSource(Point position, Color color)
{
    m_position = Point(position.getPoint());
    m_color = Color(color.getRed(), color.getGreen(), color.getBlue());
}

Point LightSource::getPosition() { return m_position; }

Color LightSource::getColor() { return m_color; }

void LightSource::setPosition(Point position) { m_position = Point(position.getPoint()); }

void LightSource::setColor(Color color) { m_color = Color(color.getRed(), color.getGreen(), color.getBlue()); }

void LightSource::transform(Eigen::Matrix4d transMat) { m_position.transform(transMat); }
