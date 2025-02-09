#include "../header/camera.h"
#include "../header/color.h"
#include "../header/constants.h"
#include "../header/light_source.h"
#include "../header/phong.h"
#include "../header/point.h"
#include "../header/sphere.h"
#include "../header/triangle.h"
#include "../header/vector.h"
#include "../header/world.h"

#include <Eigen/Dense>
#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{

    // Illumination Models
    Phong* spherePhong = new Phong(0.2, 0.4, 0.6, 2);
    Phong* sphere2Phong = new Phong(0.2, 0.4, 0.6, 2);
    Phong* floorPhong = new Phong(0.2, 0.4, 0.6, 2);
    Phong* floor2Phong = new Phong(0.2, 0.4, 0.6, 2);

    // Create objects here!!!
    // Make sure they're dynamically allocated - they will get destroyed by the world
    Sphere* sphere = new Sphere(0.1, 1, 4.5, 1);
    sphere->setColor(Color(1, 0, 0));
    sphere->setIlluminationModel(spherePhong);
    Sphere* sphere2 = new Sphere(2.3, -0.3, 2.2, 1.3);
    sphere2->setColor(Color(0, 0, 1));
    sphere2->setIlluminationModel(sphere2Phong);
    Triangle* floor1 = new Triangle(-3, -1.5, -60, -3, -1.5, 10, 7, -1.5, -60);
    floor1->setColor(Color(0, 1, 0));
    floor1->setIlluminationModel(floorPhong);
    Triangle* floor2 = new Triangle(7, -1.5, -60, -3, -1.5, 10, 7, -1.5, 10);
    floor2->setColor(Color(0, 1, 0));
    floor2->setIlluminationModel(floor2Phong);

    // Create light source here!!!
    // Light sources get destroyed by the world
    LightSource* light = new LightSource(Point(0, 8, 8), Color(1.0, 1.0, 1.0));

    // Create world and add objects + light to the scene
    // World will get destroyed after camera render
    World* world = new World();
    world->addObject(sphere);
    world->addObject(sphere2);
    world->addObject(floor1);
    world->addObject(floor2);
    world->addLightSource(light);

    // Set up the camera and render the world
    Camera* camera = new Camera(CAMERA_POS_X, CAMERA_POS_Y, CAMERA_POS_Z, CAMERA_LOOKAT_X, CAMERA_LOOKAT_Y,
        CAMERA_LOOKAT_Z, CAMERA_UP_X, CAMERA_UP_Y, CAMERA_UP_Z);
    camera->render(world);

    // Cleanup
    delete camera;

    cout << "Image generated successfully!\n";
    return 0;
}