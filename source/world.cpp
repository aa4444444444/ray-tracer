#include "../header/world.h"
#include <algorithm>

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
    std::for_each(
        m_objectList.begin(), m_objectList.end(), [ray, closestObjectColor, closestPoint](const auto& elem) mutable {
            Ray* intersection = elem->intersect(ray);
            if (intersection != nullptr) {
                if (closestPoint == nullptr) {
                    closestPoint = intersection->getOrigin();
                } else {
                    if (intersection->getOrigin()->distance(Point(0, 0, 0)) < closestPoint->distance(Point(0, 0, 0))) {
                        closestPoint = intersection->getOrigin();
                        short newRed = 255;
                        closestObjectColor = Color(newRed, newRed, 0);
                    }
                }
            }
        });

    return closestObjectColor;
}

void World::transform(Object* object, Eigen::Matrix4d transMat) { object->transform(transMat); }

void World::transformAllObjects(Eigen::Matrix4d transMat)
{
    std::for_each(
        m_objectList.begin(), m_objectList.end(), [transMat](const auto& elem) { elem->transform(transMat); });
}