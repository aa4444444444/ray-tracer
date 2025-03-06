#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H

#include "color.h"
#include "constants.h"
#include "texture.h"
#include <Eigen/Dense>

/**
 * @brief The Checkerboard texture.
 *
 */
class Checkerboard : public Texture {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Checkerboard(Color color1, Color color2, float width, float height, float checksize, float noiseScale);
    void setPerlinNoise(bool usePerlin);
    Color getColor(Intersection* intersection) override;

private:
    Color m_color1;
    Color m_color2;
    float m_width;
    float m_height;
    bool m_usePerlinNoise;
    float m_checksize;
    float m_noiseScale;
    Eigen::Matrix4d m_inverseViewTransform;
};

#endif