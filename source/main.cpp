#include "../header/color.h"
#include "../header/point.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    Color *color = new Color(204, 200, 24);
    cout << color->getBlue() << endl;
    delete color;
    return 0;
}