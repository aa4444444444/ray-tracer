#include "../header/ashikhmin_shirley.h"
#include "../header/camera.h"
#include "../header/checkerboard.h"
#include "../header/color.h"
#include "../header/cone.h"
#include "../header/constants.h"
#include "../header/cylinder.h"
#include "../header/happly.h"
#include "../header/image_texture.h"
#include "../header/light_source.h"
#include "../header/mandelbrot.h"
#include "../header/phong.h"
#include "../header/ply_model.h"
#include "../header/point.h"
#include "../header/sphere.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../header/stb_image.h"
#include "../header/triangle.h"
#include "../header/vector.h"
#include "../header/world.h"

#include <Eigen/Dense>
#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;

void addBunny(World* world, Color& color, float& scaleAmount, const Eigen::Vector3d& rotateVec,
    const Eigen::Vector3d& translateVec)
{
    // Loading in PLY files
    happly::PLYData plyIn("plys/bunny/reconstruction/bun_zipper.ply");

    // Get data from the object
    std::vector<float> vertexX = plyIn.getElement("vertex").getProperty<float>("x");
    std::vector<float> vertexY = plyIn.getElement("vertex").getProperty<float>("y");
    std::vector<float> vertexZ = plyIn.getElement("vertex").getProperty<float>("z");

    std::vector<std::vector<int>> indices = plyIn.getElement("face").getListProperty<int>("vertex_indices");

    PlyModel* p = new PlyModel(vertexX, vertexY, vertexZ, indices, color, scaleAmount, rotateVec, translateVec);

    world->addPlyModel(p);
}

void addArmadillo(World* world)
{
    // Loading in PLY files
    happly::PLYData plyIn("plys/armadillo/Armadillo.ply");

    // Get data from the object
    std::vector<float> vertexX = plyIn.getElement("vertex").getProperty<float>("x");
    std::vector<float> vertexY = plyIn.getElement("vertex").getProperty<float>("y");
    std::vector<float> vertexZ = plyIn.getElement("vertex").getProperty<float>("z");

    std::vector<std::vector<int>> indices = plyIn.getElement("face").getListProperty<int>("vertex_indices");

    PlyModel* p = new PlyModel(vertexX, vertexY, vertexZ, indices, Color(1.0f, 0.0f, 0.0f), 40.0f,
        Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, -3.0f, 0));

    world->addPlyModel(p);
}

void addMahoraga(World* world)
{
    // Loading in PLY files
    happly::PLYData plyIn("plys/mahoraga/mahoraga2.ply");

    // Get data from the object
    std::vector<float> vertexX = plyIn.getElement("vertex").getProperty<float>("x");
    std::vector<float> vertexY = plyIn.getElement("vertex").getProperty<float>("y");
    std::vector<float> vertexZ = plyIn.getElement("vertex").getProperty<float>("z");

    std::vector<std::vector<int>> indices = plyIn.getElement("face").getListProperty<int>("vertex_index");

    PlyModel* p = new PlyModel(vertexX, vertexY, vertexZ, indices, Color(1.0f, 1.0f, 1.0f), 0.3f,
        Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, -3.0f, 0));

    world->addPlyModel(p);
}

void addBallScene(World* world)
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
    // floorCheckerboard->setPerlinNoise(false);
    // Checkerboard* floor2Checkerboard
    //     = new Checkerboard(Color(1.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 0.0f), 70.0f, 10.0f, 0.1f, 1000.0f);
    // floor2Checkerboard->setPerlinNoise(false);

    ImageTexture* floorImage = new ImageTexture("textures/cowboy.png");
    ImageTexture* floor2Image = new ImageTexture("textures/cowboy.png");

    // Mandelbrot* floorMandelbrot = new Mandelbrot(100, 1.0f, -1.5f, 0.0f, 180.0f);
    // Mandelbrot* floor2Mandelbrot = new Mandelbrot(100, 1.0f, -1.5f, 0.0f, 180.0f);

    // Create objects here !!!Make sure they're dynamically allocated - they will get destroyed by the world
    Sphere* sphere = new Sphere(0.1, 1, 4.5, 1.0f);
    sphere->setColor(Color(1, 0, 0));
    sphere->setIlluminationModel(spherePhong);
    sphere->setKReflection(0.0f);
    sphere->setKTransmission(0.0f);
    sphere->setIndexOfRefraction(1.0f);
    sphere->setMaxDepth(7);

    Sphere* sphere2 = new Sphere(2.3, -0.2, 2.2, 1.3f);
    sphere2->setColor(Color(0, 0, 1));
    sphere2->setIlluminationModel(sphere2Phong);
    sphere2->setKReflection(0.3f);
    sphere2->setMaxDepth(7);

    Triangle* floor1 = new Triangle(-3, -1.5, -60, -3, -1.5, 10, 7, -1.5, -60);
    floor1->setColor(Color(0, 1, 0));
    floor1->setIlluminationModel(floorPhong);
    floor1->setTexture(floorImage);
    floor1->setTexturePoints(Eigen::Vector2d(0.0f, 0.0f), Eigen::Vector2d(1.0f, 0.0f), Eigen::Vector2d(0.0f, 1.0f));

    Triangle* floor2 = new Triangle(7, -1.5, -60, -3, -1.5, 10, 7, -1.5, 10);
    floor2->setColor(Color(0, 1, 0));
    floor2->setIlluminationModel(floor2Phong);
    floor2->setTexture(floor2Image);
    floor2->setTexturePoints(Eigen::Vector2d(0.0f, 1.0f), Eigen::Vector2d(1.0f, 0.0f), Eigen::Vector2d(1.0f, 1.0f));

    world->addModelObject(sphere);
    world->addModelObject(sphere2);
    world->addModelObject(floor1);
    world->addModelObject(floor2);
}

void addRefractionTest(World* world)
{
    // Illumination Models

    Phong* spherePhong = new Phong(0.9, 0.3, 0.65, 50);
    spherePhong->setPhongBlinn(true);

    Phong* floorPhong = new Phong(0.2, 0.7, 0.3, 2);
    Phong* floor2Phong = new Phong(0.2, 0.7, 0.3, 2);

    Mandelbrot* floorMandelbrot = new Mandelbrot(100, 1.0f, -1.5f, 0.0f, 180.0f);
    Mandelbrot* floor2Mandelbrot = new Mandelbrot(100, 1.0f, -1.5f, 0.0f, 180.0f);

    // Create objects here !!!Make sure they're dynamically allocated - they will get destroyed by the world
    Sphere* sphere = new Sphere(0.1, 1, 4.5, 1.0f);
    sphere->setColor(Color(1, 1, 1));
    sphere->setIlluminationModel(spherePhong);
    sphere->setKReflection(0.0f);
    sphere->setKTransmission(0.8f);
    sphere->setIndexOfRefraction(2.0f);
    sphere->setMaxDepth(7);

    Triangle* floor1 = new Triangle(-10, 60, -10, -10, -30, -10, 10, 60, -10);
    floor1->setColor(Color(0, 1, 0));
    floor1->setIlluminationModel(floorPhong);
    floor1->setTexture(floorMandelbrot);
    floor1->setTexturePoints(Eigen::Vector2d(0.0f, 0.0f), Eigen::Vector2d(1.0f, 0.0f), Eigen::Vector2d(0.0f, 1.0f));

    Triangle* floor2 = new Triangle(10, 60, -10, -10, -30, -10, 10, -30, -10);
    floor2->setColor(Color(0, 1, 0));
    floor2->setIlluminationModel(floor2Phong);
    floor2->setTexture(floor2Mandelbrot);
    floor2->setTexturePoints(Eigen::Vector2d(0.0f, 1.0f), Eigen::Vector2d(1.0f, 0.0f), Eigen::Vector2d(1.0f, 1.0f));

    world->addModelObject(sphere);
    world->addModelObject(floor1);
    world->addModelObject(floor2);
}

void addShapeTest(World* world)
{
    // Illumination Models

    Phong* conePhong = new Phong(0.9, 0.3, 0.65, 50);
    conePhong->setPhongBlinn(true);

    Phong* floorPhong = new Phong(0.2, 0.7, 0.3, 2);
    Phong* floor2Phong = new Phong(0.2, 0.7, 0.3, 2);

    Checkerboard* floorCheckerboard
        = new Checkerboard(Color(1.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 0.0f), 70.0f, 10.0f, 0.1f, 1000.0f);
    floorCheckerboard->setPerlinNoise(false);
    Checkerboard* floor2Checkerboard
        = new Checkerboard(Color(1.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 0.0f), 70.0f, 10.0f, 0.1f, 1000.0f);
    floor2Checkerboard->setPerlinNoise(false);

    // Create objects here !!!Make sure they're dynamically allocated - they will get destroyed by the world
    Cone* cone = new Cone(Point(0.0, 0.2, 4.0), Point(0.0, 2.0, 4.0), 1.0f);
    cone->setColor(Color(0, 0, 1));
    cone->setIlluminationModel(conePhong);
    cone->setKReflection(0.0f);
    cone->setKTransmission(0.9f);
    cone->setIndexOfRefraction(1.0f);
    cone->setMaxDepth(7);

    Triangle* floor1 = new Triangle(-3, -1.5, -60, -3, -1.5, 10, 7, -1.5, -60);
    floor1->setColor(Color(0, 1, 0));
    floor1->setIlluminationModel(floorPhong);
    floor1->setTexture(floorCheckerboard);
    floor1->setTexturePoints(Eigen::Vector2d(0.0f, 0.0f), Eigen::Vector2d(1.0f, 0.0f), Eigen::Vector2d(0.0f, 1.0f));

    Triangle* floor2 = new Triangle(7, -1.5, -60, -3, -1.5, 10, 7, -1.5, 10);
    floor2->setColor(Color(0, 1, 0));
    floor2->setIlluminationModel(floor2Phong);
    floor2->setTexture(floor2Checkerboard);
    floor2->setTexturePoints(Eigen::Vector2d(0.0f, 1.0f), Eigen::Vector2d(1.0f, 0.0f), Eigen::Vector2d(1.0f, 1.0f));

    world->addModelObject(cone);
    world->addModelObject(floor1);
    world->addModelObject(floor2);
}

int main(int argc, char* argv[])
{

    // Create the world
    // World will get destroyed after camera render
    World* world = new World();

    // addBallScene(world);
    addShapeTest(world);
    // addRefractionTest(world);

    // Create light source here!!!
    // Light sources get destroyed by the world
    LightSource* light = new LightSource(Point(0, 3, 8), Radiance(100.0, 100.0, 100.0));
    // LightSource* light2 = new LightSource(Point(4, 1, 8), Radiance(50.0, 50.0, 50.0));

    // add light to the scene
    world->addLightSource(light);
    // world->addLightSource(light2);

    // Set up the camera and render the world
    Camera* camera = new Camera(CAMERA_POS_X, CAMERA_POS_Y, CAMERA_POS_Z, CAMERA_LOOKAT_X, CAMERA_LOOKAT_Y,
        CAMERA_LOOKAT_Z, CAMERA_UP_X, CAMERA_UP_Y, CAMERA_UP_Z);
    camera->render(world);

    // Cleanup
    delete camera;

    cout << "Image generated successfully!\n";
    return 0;
}