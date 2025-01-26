#include "../header/world.h"

World::World() { }

void World::addObject(Object* object) { m_objectList.push_back(object); }

void World::spawnRay(Ray* ray) { }