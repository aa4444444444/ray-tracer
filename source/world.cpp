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
    // Default color (color of background)
    Point* closestPoint = nullptr;
    Color closestObjectColor(0, 0, 0);

    // Origin
    Point coordOrigin(0, 0, 0);

    // Loop through all the objects in the scene
    for (size_t i = 0; i < m_objectList.size(); i++) {
        Ray* intersection = m_objectList[i]->intersect(ray);
        if (intersection != nullptr) {
            if (closestPoint == nullptr) {
                // If an intersection hasn't been found yet
                closestPoint = new Point(intersection->getOrigin());
                closestObjectColor.setRed(m_objectList[i]->getColor().getRed());
                closestObjectColor.setGreen(m_objectList[i]->getColor().getGreen());
                closestObjectColor.setBlue(m_objectList[i]->getColor().getBlue());
            } else {
                // If an intersection was previously found
                // Check if new intersection is closer

                if (intersection->getOrigin()->distance(&coordOrigin) < closestPoint->distance(&coordOrigin)) {
                    delete closestPoint;
                    closestPoint = new Point(intersection->getOrigin());
                    closestObjectColor.setRed(m_objectList[i]->getColor().getRed());
                    closestObjectColor.setGreen(m_objectList[i]->getColor().getGreen());
                    closestObjectColor.setBlue(m_objectList[i]->getColor().getBlue());
                }
            }
            delete intersection;
        }
    }

    if (closestPoint != nullptr) {
        delete closestPoint;
    }

    return closestObjectColor;
}

void World::transform(Object* object, Eigen::Matrix4d transMat) { object->transform(transMat); }

void World::transformAllObjects(Eigen::Matrix4d transMat)
{
    std::for_each(
        m_objectList.begin(), m_objectList.end(), [transMat](const auto& elem) { elem->transform(transMat); });
}