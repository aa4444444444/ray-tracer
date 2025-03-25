#ifndef WORLD_H
#define WORLD_H

#include "color.h"
#include "constants.h"
#include "kd_tree_node.h"
#include "light_source.h"
#include "object.h"
#include "ply_model.h"
#include "ray.h"
#include <vector>

struct FinalRadiance {
    FinalRadiance(Radiance rad, bool intersectionFound)
        : m_radiance(rad)
        , m_intersectionFound(intersectionFound)
    {
    }
    Radiance m_radiance;
    bool m_intersectionFound;
};

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
    void addPrimitiveObject(Object* object);
    void addModelObject(Object* object);
    void addPlyModel(PlyModel* plyModel);
    void addLightSource(LightSource* lightSource);
    Radiance spawnRay(Ray* ray);
    FinalRadiance traverseKDTree(Ray* ray, KdTreeNode* treeNode);
    void transform(Object* object, Eigen::Matrix4d transMat);
    void transformAllObjects(Eigen::Matrix4d transMat);
    void transformLightSources(Eigen::Matrix4d transMat);
    void buildKDTree();
    // Using spatial median along partitioning axis
    KdTreeNode* getNode(KdTreeNode* voxel, std::vector<AxisAlignedBoundingBox*> primitives,
        int recursiveLevel = KD_MAX_RECURSION_LEVEL);
    // Using Surface Area Heuristic
    KdTreeNode* getNodeSAH(KdTreeNode* voxel, std::vector<AxisAlignedBoundingBox*>& primitives,
        int recursiveLevel = KD_MAX_RECURSION_LEVEL);

private:
    std::vector<PlyModel*> m_plyModels;
    std::vector<std::vector<Object*>*> m_modelList;
    std::vector<Object*> m_objectList;
    std::vector<LightSource*> m_lightSourceList;
    std::vector<AxisAlignedBoundingBox*> m_aabbList;
    KdTreeNode* m_sceneKDTree;

    void findOptimalSplit(std::vector<AxisAlignedBoundingBox*>& sortedPrimitives, float& currentSplitDistance,
        KdTreeNode::DimSplit& currentDimension, float& currentCost, KdTreeNode::DimSplit splitDimension,
        AxisAlignedBoundingBox* currentVoxel);
    void findOptimalSplitWithBinning(std::vector<AxisAlignedBoundingBox*>& sortedPrimitives,
        float& currentSplitDistance, KdTreeNode::DimSplit& currentDimension, float& currentCost,
        KdTreeNode::DimSplit splitDimension, AxisAlignedBoundingBox* currentVoxel);
    float computeCost(std::vector<AxisAlignedBoundingBox*>& sortedPrimitives, int direction, float splitDistance,
        float parentSurfaceArea, AxisAlignedBoundingBox* currentVoxel);
    float getSurfaceArea(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
};

#endif