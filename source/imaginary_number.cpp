#include "../header/imaginary_number.h"

ImaginaryNumber::ImaginaryNumber(double real, double imag)
{
    m_real = real;
    m_imag = imag;
}

double ImaginaryNumber::real() { return m_real; }
double ImaginaryNumber::imag() { return m_imag; }

ImaginaryNumber ImaginaryNumber::operator+(const ImaginaryNumber& other)
{
    return ImaginaryNumber(m_real + other.m_real, m_imag + other.m_imag);
}

ImaginaryNumber ImaginaryNumber::operator*(const ImaginaryNumber& other)
{
    double real_part = m_real * other.m_real - m_imag * other.m_imag;
    double imag_part = m_real * other.m_imag + m_imag * other.m_real;
    return ImaginaryNumber(real_part, imag_part);
}

double ImaginaryNumber::magnitude() { return std::sqrt(m_real * m_real + m_imag * m_imag); }