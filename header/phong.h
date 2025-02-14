#ifndef PHONG_H
#define PHONG_H

#include "constants.h"
#include "illumination_model.h"
#include "intersection.h"

/**
 * @brief The Phong Illumination Model.
 *
 */
class Phong : public IlluminationModel {
public:
    Phong(float k_a, float k_d, float k_s, float k_e);

    Radiance illuminate(Intersection* intersection) override;

    void setPhongBlinn(bool phongBlinn);

private:
    float m_ka;
    float m_kd;
    float m_ks;
    float m_ke;
    bool m_isPhongBlinn;
};

#endif