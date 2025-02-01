#include "../header/camera.h"
#include "../header/constants.h"
#include <fstream>
#include <iostream>

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
    std::ofstream outfile;

    // Attempt to open outfile
    outfile.open("output/image.ppm");

    // Verify file is open
    if (outfile.is_open()) {

        // Set up PPM file
        outfile << "P3" << std::endl;
        outfile << IMAGE_WIDTH << " " << IMAGE_HEIGHT << std::endl;
        outfile << "255" << std::endl;

        // First convert objects from world coordinates to camera coordinates
        Vector* n = new Vector(m_position->getPoint() - m_lookat->getPoint());
        n->normalize();
        Vector* u = m_up->cross(n);
        u->normalize();
        Vector* v = n->cross(u);
        Eigen::Matrix4d viewTransform;
        viewTransform << u->getVector()(0), u->getVector()(1), u->getVector()(2),
            (m_position->getPoint() * -1).dot(u->getVector()), v->getVector()(0), v->getVector()(1), v->getVector()(2),
            (m_position->getPoint() * -1).dot(v->getVector()), n->getVector()(0), n->getVector()(1), n->getVector()(2),
            (m_position->getPoint() * -1).dot(n->getVector()), 0, 0, 0, 1;
        world->transformAllObjects(viewTransform);

        delete n;
        delete u;
        delete v;

        // world location of top-left pixel of film plane is (-w/2, h/2, f)
        // where w = width of film plane, h = height of film plane, f = focal length
        // to shoot a ray through the center of this pixel, we shift right and down
        // half a pixel, where each pixel is (h/H) tall and (w/W) wide.
        // H and W represent the height and width of the image in pixels
        float pixelWidth = FILM_PLANE_WIDTH / (IMAGE_WIDTH * 1.0);
        float pixelHeight = FILM_PLANE_HEIGHT / (IMAGE_HEIGHT * 1.0);

        Eigen::Vector3d* topLeftRay = new Eigen::Vector3d(
            -1 * FILM_PLANE_WIDTH / 2.0 + pixelWidth / 2.0, FILM_PLANE_HEIGHT / 2.0 - pixelHeight / 2.0, -FOCAL_LENGTH);

        // Loop through the film plane
        for (int i = 0; i < IMAGE_HEIGHT; i++) {
            for (int j = 0; j < IMAGE_WIDTH; j++) {
                // 0, 0, 0 since we are in camera coordinates and starting from the camera
                // std::cout << "Spawning a ray going through: (" << (*topLeftRay)(0) + j * pixelWidth << ", "
                // << (*topLeftRay)(1) - i * pixelHeight << ", " << (*topLeftRay)(2) << ")" << std::endl;
                Ray* ray = new Ray(
                    0, 0, 0, (*topLeftRay)(0) + j * pixelWidth, (*topLeftRay)(1) - i * pixelHeight, (*topLeftRay)(2));
                // std::cout << "Ray Created without issue" << std::endl;
                Color c = world->spawnRay(ray);
                // std::cout << "Spawned ray in world without issue" << std::endl;
                outfile << c.getRed() << " " << c.getGreen() << " " << c.getBlue() << " ";
                // std::cout << "Wrote to outfile without issue" << std::endl;
                delete ray;
                // std::cout << "Deleted ray without issue" << std::endl;
            }
            outfile << std::endl;
        }

        // Need to verify that intersection happens at a point past the film plane

        outfile.close();
        delete topLeftRay;
        delete world;
    } else {
        std::cerr << "COULD NOT OPEN FILE" << std::endl;
    }
}