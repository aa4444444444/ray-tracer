#include "../header/phong.h"

Phong::Phong(float k_a, float k_d, float k_s, float k_e)
{
    m_ka = k_a;
    m_kd = k_d;
    m_ks = k_s;
    m_ke = k_e;
}

void Phong::illuminate() { }