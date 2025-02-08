#ifndef PHONG_H
#define PHONG_H

#include "illumination_model.h"

/**
 * @brief The Phong Illumination Model.
 *
 */
class Phong : public IlluminationModel {
public:
    Phong(float k_a, float k_d, float k_s, float k_e);

    void illuminate() override;

private:
    float m_ka;
    float m_kd;
    float m_ks;
    float m_ke;
};

#endif