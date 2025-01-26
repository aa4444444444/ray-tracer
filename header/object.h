#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include <string>

class Object {
public:
    virtual Ray* intersect(Ray* ray) = 0;
    virtual ~Object() {};

private:
    std::string material;
};

#endif