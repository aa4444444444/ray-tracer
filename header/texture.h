#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "intersection.h"

/**
 * @brief Parent class for textures.
 */
class Texture {
public:
    virtual Color getColor(Intersection* intersection) = 0;
    virtual ~Texture() { };
};

#endif