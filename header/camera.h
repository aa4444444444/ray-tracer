#ifndef CAMERA_H
#define CAMERA_H

#include "point.h"
#include "vector.h"
#include "world.h"

/**
 * @brief The camera.
 *
 * The camera is placed in the world and is the origin of the rays which hit the scene.
 * The camera is responsible for rednering the world, which is where all the ray tracing happens.
 */
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