#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include <string>

class Object {
public:
    virtual Ray* intersect(Ray* ray) = 0;
    virtual void transform(Eigen::Matrix4d transMat) = 0;
    virtual ~Object() {};

private:
    std::string material;
};

#endif