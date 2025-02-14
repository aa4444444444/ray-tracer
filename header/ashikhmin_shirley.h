#ifndef ASHIKHMIN_SHIRLEY_H
#define ASHIKHMIN_SHIRLEY_H

#include "constants.h"
#include "illumination_model.h"
#include "intersection.h"

/**
 * @brief The Ashikhmin-Shirley Illumination Model.
 *
 */
class AshikhminShirley : public IlluminationModel {
public:
    AshikhminShirley(float n_u, float n_v, float r_s, float r_d);
    float schlickApprox(float kDotH);
    Radiance illuminate(Intersection* intersection) override;

private:
    float m_nu;
    float m_nv;
    float m_rs;
    float m_rd;
};

#endif