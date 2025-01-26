#include "../header/color.h"
#include "../header/point.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    Color *color = new Color(204, 200, 24);
    Point *point = new Point(234.23, 134.556, 2508.248);
    cout << color->getBlue() << endl;
    cout << point->getX() << endl;
    delete color;
    return 0;
}