#ifndef CAMERA_H
#define CAMERA_H

#include "point.h"
#include "vector.h"
#include "world.h"

class Camera {
public:
    Camera(float positionX, float positionY, float positionZ, float lookatX, float lookatY, float lookatZ, float upX,
        float upY, float upZ);
    void render(World* world);

private:
    Point m_position;
    Point m_lookat;
    Vector m_up;
};

#endif