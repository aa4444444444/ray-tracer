#ifndef WORLD_H
#define WORLD_H

#include "color.h"
#include "object.h"
#include "ray.h"
#include <vector>

class World {
public:
    World();
    ~World();
    void addObject(Object* object);
    Color spawnRay(Ray* ray);
    void transform(Object* object, Eigen::Matrix4d transMat);
    void transformAllObjects(Eigen::Matrix4d transMat);

private:
    std::vector<Object*> m_objectList;
};

#endif