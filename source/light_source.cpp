#include "../header/light_source.h"

LightSource::LightSource(Point position, Radiance radiance)
{
    m_position = Point(position.getPoint());
    m_radiance = Radiance(radiance.getRadianceRed(), radiance.getRadianceGreen(), radiance.getRadianceBlue());
}

Point LightSource::getPosition() { return m_position; }

Radiance LightSource::getRadiance() { return m_radiance; }

void LightSource::setPosition(Point position) { m_position = Point(position.getPoint()); }

void LightSource::setRadiance(Radiance radiance)
{
    m_radiance = Radiance(radiance.getRadianceRed(), radiance.getRadianceGreen(), radiance.getRadianceBlue());
}

void LightSource::transform(Eigen::Matrix4d transMat) { m_position.transform(transMat); }
