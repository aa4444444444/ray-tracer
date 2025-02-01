#include "../header/camera.h"
#include "../header/color.h"
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
    // Create objects here!!!
    // Make sure they're dynamically allocated - they will get destroyed by the world
    Sphere* sphere = new Sphere(0.1, 1, 4.5, 1);
    sphere->setColor(Color(255, 0, 0));
    Sphere* sphere2 = new Sphere(2.3, -0.3, 2.2, 1.3);
    sphere2->setColor(Color(0, 0, 255));
    Triangle* floor1 = new Triangle(-3, -1.5, -60, -3, -1.5, 10, 7, -1.5, -60);
    floor1->setColor(Color(0, 255, 0));
    Triangle* floor2 = new Triangle(7, -1.5, -60, -3, -1.5, 10, 7, -1.5, 10);
    floor2->setColor(Color(0, 255, 0));

    // Create world and add objects to the scene
    // World will get destroyed after camera render
    World* world = new World();
    world->addObject(sphere);
    world->addObject(sphere2);
    world->addObject(floor1);
    world->addObject(floor2);

    // Set up the camera and render the world
    Camera* camera = new Camera(0, 1, 7, 0, 0, 0, 0, 1, 0);
    camera->render(world);

    // Cleanup
    delete camera;

    cout << "Image generated successfully!\n";
    return 0;
}