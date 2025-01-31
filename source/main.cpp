#include "../header/camera.h"
#include "../header/color.h"
#include "../header/point.h"
#include "../header/sphere.h"
#include "../header/vector.h"
#include "../header/world.h"

#include <Eigen/Dense>
#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    Sphere* sphere = new Sphere(0.1, 0.2, 4, 1);
    World* world = new World();
    world->addObject(sphere);
    Camera* camera = new Camera(0, 0, 8, 0, 0, 0, 0, 1, 0);
    camera->render(world);

    std::cout << " WTF" << std::endl;
    return 0;
}