#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "stb_image.h"
#include "texture.h"

class ImageTexture : public Texture {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    ImageTexture(const std::string& filename);
    ~ImageTexture();
    Color getColor(Intersection* intersection) override;

private:
    unsigned char* m_data;
    int m_width, m_height, m_channels;
    Eigen::Matrix4d m_inverseViewTransform;
};

#endif