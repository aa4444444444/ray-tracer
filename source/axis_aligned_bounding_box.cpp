#include "../header/axis_aligned_bounding_box.h"

AxisAlignedBoundingBox::AxisAlignedBoundingBox()
{
    m_xMin = 0.0f, m_xMax = 0.0f, m_yMin = 0.0f, m_yMax = 0.0f, m_zMin = 0.0f, m_zMax = 0.0f;
}
float AxisAlignedBoundingBox::getXMin() { return m_xMin; }
float AxisAlignedBoundingBox::getXMax() { return m_xMax; }
float AxisAlignedBoundingBox::getYMin() { return m_yMin; }
float AxisAlignedBoundingBox::getYMax() { return m_yMax; }
float AxisAlignedBoundingBox::getZMin() { return m_zMin; }
float AxisAlignedBoundingBox::getZMax() { return m_zMax; }
void AxisAlignedBoundingBox::setXMin(float xMin) { m_xMin = xMin; }
void AxisAlignedBoundingBox::setXMax(float xMax) { m_xMax = xMax; }
void AxisAlignedBoundingBox::setYMin(float yMin) { m_yMin = yMin; }
void AxisAlignedBoundingBox::setYMax(float yMax) { m_yMax = yMax; }
void AxisAlignedBoundingBox::setZMin(float zMin) { m_zMin = zMin; }
void AxisAlignedBoundingBox::setZMax(float zMax) { m_zMax = zMax; }