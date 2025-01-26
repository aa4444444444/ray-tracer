#include "../header/camera.h"

Camera::Camera(float positionX, float positionY, float positionZ, float lookatX, float lookatY, float lookatZ,
    float upX, float upY, float upZ)
{
    m_position = new Point(positionX, positionY, positionZ);
    m_lookat = new Point(lookatX, lookatY, lookatZ);
    m_up = new Vector(upX, upY, upZ);
}

void Camera::render(World* world) { delete world; }