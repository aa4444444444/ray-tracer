#include "../header/image_texture.h"
#include "../header/constants.h"
#include "../header/object.h"

ImageTexture::ImageTexture(const std::string& filename)
{
    m_data = stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, 3);
    if (!m_data) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }

    // Calculate the inverse view transform
    Point position = Point(CAMERA_POS_X, CAMERA_POS_Y, CAMERA_POS_Z);
    Point lookat = Point(CAMERA_LOOKAT_X, CAMERA_LOOKAT_Y, CAMERA_LOOKAT_Z);
    Vector up = Vector(CAMERA_UP_X, CAMERA_UP_Y, CAMERA_UP_Z);
    Vector n(position.getPoint() - lookat.getPoint());
    n.normalize();
    Vector u = up.cross(&n);
    u.normalize();
    Vector v = n.cross(&u);

    Eigen::Matrix4d viewTransform;
    viewTransform << u.getVector()(0), u.getVector()(1), u.getVector()(2),
        (position.getPoint() * -1).dot(u.getVector()), v.getVector()(0), v.getVector()(1), v.getVector()(2),
        (position.getPoint() * -1).dot(v.getVector()), n.getVector()(0), n.getVector()(1), n.getVector()(2),
        (position.getPoint() * -1).dot(n.getVector()), 0, 0, 0, 1;

    m_inverseViewTransform = viewTransform.inverse();
}

ImageTexture::~ImageTexture()
{
    if (m_data) {
        stbi_image_free(m_data);
    }
}

Color ImageTexture::getColor(Intersection* intersection)
{
    // Intersection will be in camera coordinates
    // First go from camera to world
    Point objectCoordinate = intersection->getTransformedIntersectionPoint(m_inverseViewTransform);

    // Get texture coordintaes
    Eigen::Vector2d textureUV = intersection->getObject()->getTextureUV(objectCoordinate);

    float u = textureUV(0);
    float v = textureUV(1);

    // Flip v because image origin is top-left
    int x = static_cast<int>(u * m_width);
    int y = static_cast<int>(v * m_height);

    if (x < 0)
        x = 0;
    if (x >= m_width)
        x = m_width - 1;
    if (y < 0)
        y = 0;
    if (y >= m_height)
        y = m_height - 1;

    int index = (y * m_width + x) * 3;

    float r = m_data[index] / 255.0f;
    float g = m_data[index + 1] / 255.0f;
    float b = m_data[index + 2] / 255.0f;

    return Color(r, g, b);
}