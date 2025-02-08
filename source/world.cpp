#include "../header/world.h"
#include "../header/constants.h"
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

    std::for_each(m_lightSourceList.begin(), m_lightSourceList.end(), [](const auto& elem) {
        if (elem != nullptr) {
            delete elem;
        }
    });
}

void World::addObject(Object* object) { m_objectList.push_back(object); }

void World::addLightSource(LightSource* lightSource) { m_lightSourceList.push_back(lightSource); }

Color World::spawnRay(Ray* ray)
{
    // Default color (color of background)
    Point* closestPoint = nullptr;
    Color closestObjectColor(0.0, 0.0, 0.0);

    // Origin
    Point coordOrigin(0, 0, 0);

    // Loop through all the objects in the scene
    for (size_t i = 0; i < m_objectList.size(); i++) {
        Intersection* intersection = m_objectList[i]->intersect(ray);
        if (intersection != nullptr) {
            if (closestPoint == nullptr) {
                // If an intersection hasn't been found yet
                Point intersectionOrigin = intersection->getIntersectionPoint();
                closestPoint = new Point(intersectionOrigin.getPoint());
                closestObjectColor.setRed(m_objectList[i]->getColor().getRed());
                closestObjectColor.setGreen(m_objectList[i]->getColor().getGreen());
                closestObjectColor.setBlue(m_objectList[i]->getColor().getBlue());
            } else {
                // If an intersection was previously found
                // Check if new intersection is closer

                if (intersection->getIntersectionPoint().distance(&coordOrigin)
                    < closestPoint->distance(&coordOrigin)) {
                    delete closestPoint;
                    closestPoint = new Point(intersection->getIntersectionPoint());
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
    std::for_each(m_objectList.begin(), m_objectList.end(), [transMat](const auto& elem) {
        if (elem != nullptr) {
            elem->transform(transMat);
        }
    });
}

void World::transformLightSources(Eigen::Matrix4d transMat)
{
    std::for_each(m_lightSourceList.begin(), m_lightSourceList.end(), [transMat](const auto& elem) {
        if (elem != nullptr) {
            elem->transform(transMat);
        }
    });
}