#ifndef IMAGINARY_NUMBER_H
#define IMAGINARY_NUMBER_H

#include <cmath>

class ImaginaryNumber {
public:
    ImaginaryNumber(double real = 0.0, double imag = 0.0);

    double real();
    double imag();

    ImaginaryNumber operator+(const ImaginaryNumber& other);

    ImaginaryNumber operator*(const ImaginaryNumber& other);

    double magnitude();

private:
    double m_real;
    double m_imag;
};

#endif
