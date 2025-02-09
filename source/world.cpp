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

Radiance World::spawnRay(Ray* ray)
{
    Intersection* closestIntersection = nullptr;

    // Need to keep track of the closest object so that we can access its Illumination Model
    Object* closestObject = nullptr;

    // Default color (color of background)
    Radiance closestObjectRadiance(BACKGROUND_RADIANCE_RED, BACKGROUND_RADIANCE_GREEN, BACKGROUND_RADIANCE_BLUE);

    // Origin
    Point coordOrigin(0, 0, 0);

    // Loop through all the objects in the scene
    for (size_t i = 0; i < m_objectList.size(); i++) {
        Intersection* intersection = m_objectList[i]->intersect(ray);
        if (intersection != nullptr) {
            if (closestIntersection == nullptr) {
                // If an intersection hasn't been found yet
                closestIntersection = intersection;
                closestObject = m_objectList[i];
            } else {
                // If an intersection was previously found
                // Check if new intersection is closer
                if (intersection->getIntersectionPoint().distance(&coordOrigin)
                    < closestIntersection->getIntersectionPoint().distance(&coordOrigin)) {
                    delete closestIntersection;
                    closestIntersection = intersection;
                    closestObject = m_objectList[i];
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
        viewingDirection.scale(-1);
        closestIntersection->setViewingDirection(viewingDirection);

        bool isShadow = false;

        // std::cout << "Closest Object: " << closestObject << std::endl;

        // Looping through the rest of the object list to see if the point is illuminated
        // If not illuminated, point is 'in shadow'
        for (size_t i = 0; i < m_objectList.size(); i++) {
            if (closestObject != m_objectList[i]) {

                // As of right now, only checks the first light source
                LightSource* light = m_lightSourceList[0];
                Vector sourceVector
                    = Vector(light->getPosition().getPoint() - closestIntersection->getIntersectionPoint().getPoint());
                sourceVector.normalize();
                Ray* lightSourceRay = new Ray(closestIntersection->getIntersectionPoint(), sourceVector);

                Intersection* lightObjectIntersection = m_objectList[i]->intersect(lightSourceRay);

                if (lightObjectIntersection != nullptr) {
                    isShadow = true;
                }

                delete lightObjectIntersection;
                delete lightSourceRay;
            }
        }

        if (isShadow) {
            closestObjectRadiance
                = closestObject->getIlluminationModel()->illuminateInShadow(closestObject->getColor());
        } else {
            closestObjectRadiance = closestObject->getIlluminationModel()->illuminate(
                closestObject->getColor(), closestObject->getSpecColor(), closestIntersection, m_lightSourceList);
        }

        delete closestIntersection;
    }

    return closestObjectRadiance;
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