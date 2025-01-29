#include "../header/camera.h"
#include "../header/constants.h"

Camera::Camera(float positionX, float positionY, float positionZ, float lookatX, float lookatY, float lookatZ,
    float upX, float upY, float upZ)
{
    m_position = new Point(positionX, positionY, positionZ);
    m_lookat = new Point(lookatX, lookatY, lookatZ);
    m_up = new Vector(upX, upY, upZ);
}

Camera::~Camera()
{
    delete m_position;
    delete m_lookat;
    delete m_up;
}

void Camera::render(World* world)
{
    // First convert objects from world coordinates to camera coordinates
    Vector n(m_position->getPoint() - m_lookat->getPoint());
    Vector u = m_up->cross(Vector(n));
    Vector v = Vector(n.cross(u));
    Eigen::Matrix4d viewTransform;
    viewTransform << u.getVector()(0), v.getVector()(0), n.getVector()(0), 0, u.getVector()(1), v.getVector()(1),
        n.getVector()(1), 0, u.getVector()(2), v.getVector()(2), n.getVector()(2), 0,
        (m_position->getPoint() * -1).dot(u.getVector()), (m_position->getPoint() * -1).dot(v.getVector()),
        (m_position->getPoint() * -1).dot(n.getVector()), 1;
    world->transformAllObjects(viewTransform);

    // world location of top-left pixel of film plane is (-w/2, h/2, f)
    // where w = width of film plane, h = height of film plane, f = focal length
    // to shoot a ray through the center of this pixel, we shift right and down
    // half a pixel, where each pixel is (h/H) tall and (w/W) wide.
    // H and W represent the height and width of the image in pixels
    Eigen::Vector3d topLeftRay(-1 * FILM_PLANE_WIDTH / 2.0, FILM_PLANE_HEIGHT / 2.0, FOCAL_LENGTH);

    // 0, 0, 0 since we are in camera coordinates and starting from the camera
    Ray ray(0, 0, 0, topLeftRay(0), topLeftRay(1), topLeftRay(2));

    // Need to verify that intersection happens at a point past the film plane

    delete world;
}