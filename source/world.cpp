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
    Intersection* closestIntersection = nullptr;

    // Need to keep track of the closest object so that we can access its Illumination Model
    // Object* closestObject = nullptr;

    // Default color (color of background)
    Color closestObjectColor(0.0, 0.0, 0.0);

    // Origin
    Point coordOrigin(0, 0, 0);

    // Loop through all the objects in the scene
    for (size_t i = 0; i < m_objectList.size(); i++) {
        Intersection* intersection = m_objectList[i]->intersect(ray);
        if (intersection != nullptr) {
            if (closestIntersection == nullptr) {
                // If an intersection hasn't been found yet
                closestIntersection = intersection;
                closestObjectColor.setRed(m_objectList[i]->getColor().getRed());
                closestObjectColor.setGreen(m_objectList[i]->getColor().getGreen());
                closestObjectColor.setBlue(m_objectList[i]->getColor().getBlue());
                // closestObject = m_objectList[i];
            } else {
                // If an intersection was previously found
                // Check if new intersection is closer

                if (intersection->getIntersectionPoint().distance(&coordOrigin)
                    < closestIntersection->getIntersectionPoint().distance(&coordOrigin)) {
                    delete closestIntersection;
                    closestIntersection = intersection;
                    closestObjectColor.setRed(m_objectList[i]->getColor().getRed());
                    closestObjectColor.setGreen(m_objectList[i]->getColor().getGreen());
                    closestObjectColor.setBlue(m_objectList[i]->getColor().getBlue());
                    // closestObject = m_objectList[i];
                } else {
                    delete intersection;
                }
            }
        }
    }

    // At this point, closest intersection has been found OR there was no intersection
    // We proceed by applying a BRDF
    if (closestIntersection != nullptr) {
        Vector viewingDirection = ray->getDirection();
        viewingDirection.normalize();
        closestIntersection->setViewingDirection(viewingDirection);

        delete closestIntersection;
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