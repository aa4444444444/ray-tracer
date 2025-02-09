#ifndef ILLUMINATION_MODEL_H
#define ILLUMINATION_MODEL_H

#include "color.h"
#include "intersection.h"
#include "light_source.h"
#include "radiance.h"
#include <vector>

/**
 * @brief Illumination Model.
 *
 * Represents an Illumination Model.
 */
class IlluminationModel {
public:
    virtual Radiance illuminate(
        Color objectColor, Color specColor, Intersection* intersection, std::vector<LightSource*> lightSources)
        = 0;
    virtual Radiance illuminateInShadow(Color objectColor) = 0;
    virtual ~IlluminationModel() { };
};

#endif