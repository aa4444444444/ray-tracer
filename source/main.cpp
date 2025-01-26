#include "../header/color.h"
#include "../header/point.h"
#include "../header/sphere.h"
#include "../header/vector.h"

#include <Eigen/Dense>
#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    Sphere* sphere = new Sphere(1, 2, 3, 4);
    delete sphere;
    return 0;
}