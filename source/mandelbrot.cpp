#include "../header/mandelbrot.h"
#include "../header/constants.h"
#include "../header/object.h"
#include <iostream>

void Mandelbrot::setupInverseMatrixTransform()
{
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

Mandelbrot::Mandelbrot()
{
    m_maxIterations = 100;
    m_zoom = 1.0;
    m_offsetX = -1.5;
    m_offsetY = 0.0;
    m_hueOffset = 180.0;

    setupInverseMatrixTransform();
}

Mandelbrot::Mandelbrot(int maxIterations, double zoom, double offsetX, double offsetY, float hueOffset)
{
    m_maxIterations = maxIterations;
    m_zoom = zoom;
    m_offsetX = offsetX;
    m_offsetY = offsetY;
    m_hueOffset = hueOffset;

    setupInverseMatrixTransform();
}

int Mandelbrot::mandelbrotIterations(double x, double y, ImaginaryNumber& z)
{
    ImaginaryNumber c(x, y);
    int iterations = 0;

    while (z.magnitude() * z.magnitude() <= 4.0 && iterations < m_maxIterations) {
        z = z * z + c;
        iterations++;
    }

    return iterations;
}

Color hsvToRgb(double h, double s, double v)
{
    double c = v * s;
    double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    double m = v - c;

    double r, g, b;
    if (h < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (h < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (h < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (h < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (h < 300) {
        r = x;
        g = 0;
        b = c;
    } else {
        r = c;
        g = 0;
        b = x;
    }

    return Color(r + m, g + m, b + m);
}

Color Mandelbrot::getColor(Intersection* intersection)
{
    // Intersection will be in camera coordinates
    // First, go from camera to world coordinates
    Point objectCoordinate = intersection->getTransformedIntersectionPoint(m_inverseViewTransform);

    // Get texture coordinates (UV)
    Eigen::Vector2d textureUV = intersection->getObject()->getTextureUV(objectCoordinate);

    // Map texture coordinates to complex plane
    double x = (textureUV(0) - 0.5) * 4.0 * m_zoom + m_offsetX;
    double y = (textureUV(1) - 0.5) * 3.0 * m_zoom + m_offsetY;
    ImaginaryNumber z(0.0f, 0.0f);

    int iterations = mandelbrotIterations(x, y, z);

    if (iterations == m_maxIterations) {
        return Color(0.0f, 0.0f, 0.0f);
    }

    double hue = 360.0 * iterations / (m_maxIterations * 1.0f);
    return hsvToRgb(hue + m_hueOffset, 1.0, 1.0);
}
