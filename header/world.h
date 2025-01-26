#ifndef WORLD_H
#define WORLD_H

#include "object.h"
#include "ray.h"
#include <vector>

class World {
public:
    World();
    void addObject(Object* object);
    void spawnRay(Ray* ray);

private:
    std::vector<Object*> m_objectList;
};

#endif