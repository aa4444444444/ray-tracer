#include "../header/checkerboard.h"
#include "../header/object.h"
#include "../header/perlin_noise.h"
#include <random>

Checkerboard::Checkerboard(Color color1, Color color2, float width, float height, float checksize)
{
    m_color1 = color1;
    m_color2 = color2;
    m_checksize = checksize;
    m_width = width;
    m_height = height;

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

void Checkerboard::setPerlinNoise(bool usePerlin) { m_usePerlinNoise = usePerlin; }

Color Checkerboard::getColor(Intersection* intersection)
{
    // Intersection will be in camera coordinates
    // First go from camera to world
    Point objectCoordinate = intersection->getTransformedIntersectionPoint(m_inverseViewTransform);

    // Get texture coordintaes
    Eigen::Vector2d textureUV = intersection->getObject()->getTextureUV(objectCoordinate);

    int row = (int)floor(textureUV(0) * (m_width / m_height) / m_checksize) % 2;
    int col = (int)floor(textureUV(1) / m_checksize) % 2;

    Color color = ((row == 0 && col == 0) || (row == 1 && col == 1)) ? m_color1 : m_color2;

    if (m_usePerlinNoise) {
        std::random_device rd;
        std::mt19937 gen(rd()); // Mersenne Twister RNG with the random seed
        std::uniform_int_distribution<> dist(0, 1000000); // Range for the seed
        int randomSeed = dist(gen);

        PerlinNoise perlin(randomSeed);
        double noiseValue = perlin.noise(textureUV(0) * 1000.0, textureUV(1) * 1000.0, 0.0);
        double variation = 0.5 + (noiseValue * 0.5);
        color *= variation;
    }

    return color;
}