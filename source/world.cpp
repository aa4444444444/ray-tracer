#include "../header/world.h"
#include <algorithm>

World::World() { }

World::~World()
{
    std::for_each(m_objectList.begin(), m_objectList.end(), [](const auto& elem) { delete elem; });
}

void World::addObject(Object* object) { m_objectList.push_back(object); }

void World::spawnRay(Ray* ray) { }