#ifndef AXIS_ALIGNED_BOUNDING_BOX_H
#define AXIS_ALIGNED_BOUNDING_BOX_H

class AxisAlignedBoundingBox {
public:
    AxisAlignedBoundingBox();
    float getXMin();
    float getXMax();
    float getYMin();
    float getYMax();
    float getZMin();
    float getZMax();
    void setXMin(float xMin);
    void setXMax(float xMax);
    void setYMin(float yMin);
    void setYMax(float yMax);
    void setZMin(float zMin);
    void setZMax(float zMax);

private:
    float m_xMin, m_xMax;
    float m_yMin, m_yMax;
    float m_zMin, m_zMax;
};

#endif