#include "../header/ashikhmin_shirley.h"
#include "../header/camera.h"
#include "../header/checkerboard.h"
#include "../header/color.h"
#include "../header/constants.h"
#include "../header/light_source.h"
#include "../header/mandelbrot.h"
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

    Phong* spherePhong = new Phong(0.9, 0.3, 0.65, 50);
    spherePhong->setPhongBlinn(true);
    Phong* sphere2Phong = new Phong(0.9, 0.3, 0.65, 50);
    sphere2Phong->setPhongBlinn(true);
    // AshikhminShirley* sphereAshikhmin = new AshikhminShirley(100, 100, 0.1f, 0.9f);
    // AshikhminShirley* sphere2Ashikhmin = new AshikhminShirley(100, 100, 0.1f, 0.9f);
    Phong* floorPhong = new Phong(0.2, 0.7, 0.3, 2);
    Phong* floor2Phong = new Phong(0.2, 0.7, 0.3, 2);
    // Checkerboard* floorCheckerboard
    //     = new Checkerboard(Color(1.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 0.0f), 70.0f, 10.0f, 0.1f, 1000.0f);
    // floorCheckerboard->setPerlinNoise(true);
    // Checkerboard* floor2Checkerboard
    //     = new Checkerboard(Color(1.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 0.0f), 70.0f, 10.0f, 0.1f, 1000.0f);
    // floor2Checkerboard->setPerlinNoise(true);
    Mandelbrot* floorMandelbrot = new Mandelbrot(100, 1.0f, -1.5f, 0.0f, 180.0f);
    Mandelbrot* floor2Mandelbrot = new Mandelbrot(100, 1.0f, -1.5f, 0.0f, 180.0f);

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
    floor1->setTexture(floorMandelbrot);
    floor1->setTexturePoints(Eigen::Vector2d(0.0f, 0.0f), Eigen::Vector2d(1.0f, 0.0f), Eigen::Vector2d(0.0f, 1.0f));

    Triangle* floor2 = new Triangle(7, -1.5, -60, -3, -1.5, 10, 7, -1.5, 10);
    floor2->setColor(Color(0, 1, 0));
    floor2->setIlluminationModel(floor2Phong);
    floor2->setTexture(floor2Mandelbrot);
    floor2->setTexturePoints(Eigen::Vector2d(0.0f, 1.0f), Eigen::Vector2d(1.0f, 0.0f), Eigen::Vector2d(1.0f, 1.0f));

    // Create light source here!!!
    // Light sources get destroyed by the world
    LightSource* light = new LightSource(Point(0, 8, 8), Radiance(100.0, 100.0, 100.0));

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