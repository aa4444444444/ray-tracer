#include "../header/world.h"
#include <algorithm>
#include <iostream>

World::World() { }

World::~World()
{
    std::for_each(m_objectList.begin(), m_objectList.end(), [](const auto& elem) {
        if (elem != nullptr) {
            delete elem;
        }
    });
}

void World::addObject(Object* object) { m_objectList.push_back(object); }

Color World::spawnRay(Ray* ray)
{
    Point* closestPoint = nullptr;
    Color closestObjectColor(0, 0, 0);

    for (size_t i = 0; i < m_objectList.size(); i++) {
        Ray* intersection = m_objectList[i]->intersect(ray);
        if (intersection != nullptr) {
            // std::cout << "INTERSECTOIN FOUND!!!" << std::endl;
            // std::cout << intersection->getOrigin()->getPoint() << std::endl;
            // std::cout << intersection->getDirection()->getVector() << std::endl;
            if (closestPoint == nullptr) {
                closestPoint = new Point(intersection->getOrigin());
                closestObjectColor.setRed(255);
                closestObjectColor.setBlue(255);
                // std::cout << "NEW POINT: " << closestPoint->getPoint() << std::endl;
            } else {
                // std::cout << "closest object was not nullptr!" << std::endl;
                Point* coordOrigin = new Point(0, 0, 0);
                if (intersection->getOrigin()->distance(coordOrigin) < closestPoint->distance(coordOrigin)) {
                    delete closestPoint;
                    closestPoint = new Point(intersection->getOrigin());
                    closestObjectColor.setRed(255);
                    closestObjectColor.setBlue(255);
                }
                delete coordOrigin;
            }
        }
        delete intersection;
    }

    if (closestPoint == nullptr) {
        // std::cout << "CLOSEST POINT IS NULL" << std::endl;
    } else {
        // std::cout << "Closest Point: " << closestPoint->getPoint() << std::endl;
        // std::cout << "Color: " << closestObjectColor.getRed() << " " << closestObjectColor.getGreen() << " "
        //   << closestObjectColor.getBlue() << std::endl;
    }

    return closestObjectColor;
}

void World::transform(Object* object, Eigen::Matrix4d transMat) { object->transform(transMat); }

void World::transformAllObjects(Eigen::Matrix4d transMat)
{
    std::for_each(
        m_objectList.begin(), m_objectList.end(), [transMat](const auto& elem) { elem->transform(transMat); });
}