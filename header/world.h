#ifndef WORLD_H
#define WORLD_H

#include "color.h"
#include "light_source.h"
#include "object.h"
#include "ray.h"
#include <vector>

/**
 * @brief The world seen by the camera.
 *
 * World objects are responsible for keeping track of any objects in the scene,
 * as well as spawning rays into the scene.
 */
class World {
public:
    World();
    ~World();
    void addObject(Object* object);
    void addLightSource(LightSource* lightSource);
    Color spawnRay(Ray* ray);
    void transform(Object* object, Eigen::Matrix4d transMat);
    void transformAllObjects(Eigen::Matrix4d transMat);
    void transformLightSources(Eigen::Matrix4d transMat);

private:
    std::vector<Object*> m_objectList;
    std::vector<LightSource*> m_lightSourceList;
};

#endif