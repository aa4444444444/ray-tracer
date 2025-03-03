#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "color.h"
#include "imaginary_number.h"
#include "texture.h"

class Mandelbrot : public Texture {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Mandelbrot();
    Mandelbrot(int maxIterations, double zoom, double offsetX, double offsetY);
    Color getColor(Intersection* intersection) override;

private:
    int m_maxIterations;
    double m_zoom;
    double m_offsetX;
    double m_offsetY;
    Eigen::Matrix4d m_inverseViewTransform;
    int mandelbrotIterations(double x, double y, ImaginaryNumber& z);
};
#endif
