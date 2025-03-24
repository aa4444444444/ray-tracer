#include "../header/world.h"
#include <algorithm>
#include <iostream>
#include <limits>

World::World() { m_sceneKDTree = nullptr; }

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

    std::for_each(m_aabbList.begin(), m_aabbList.end(), [](const auto& elem) {
        if (elem != nullptr) {
            delete elem;
        }
    });

    std::for_each(m_plyModels.begin(), m_plyModels.end(), [](const auto& elem) {
        if (elem != nullptr) {
            delete elem;
        }
    });

    std::for_each(m_modelList.begin(), m_modelList.end(), [](const auto& elem) {
        if (elem != nullptr) {
            delete elem;
        }
    });

    if (m_sceneKDTree != nullptr) {
        delete m_sceneKDTree;
    }
}

void World::addPrimitiveObject(Object* object) { m_objectList.push_back(object); }

void World::addModelObject(Object* object)
{
    std::vector<Object*>* model = new std::vector<Object*> { object };
    m_modelList.push_back(model);
    object->setModelAddress(model);
    m_objectList.push_back(object);
}

void World::addPlyModel(PlyModel* plyModel)
{
    std::vector<Object*>* model = new std::vector<Object*>(plyModel->getPrimitives());
    m_modelList.push_back(model);
    for (const auto& elem : *model) {
        elem->setModelAddress(model);
        m_objectList.push_back(elem);
    }
    m_plyModels.push_back(plyModel);
}

void World::addLightSource(LightSource* lightSource) { m_lightSourceList.push_back(lightSource); }

FinalRadiance World::traverseKDTree(Ray* ray, KdTreeNode* treeNode)
{
    // If leaf we just find intersection of objects in bounding box
    if (treeNode->getIsLeaf()) {
        Intersection* closestIntersection = nullptr;

        // Need to keep track of the closest object so that we can access its Illumination Model
        Object* closestObject = nullptr;

        // Default color (color of background)
        Radiance closestObjectRadiance(BACKGROUND_RADIANCE_RED, BACKGROUND_RADIANCE_GREEN, BACKGROUND_RADIANCE_BLUE);

        // Origin
        Point coordOrigin(0, 0, 0);

        bool intersectionFound = false;

        // Loop through all the objects in the AABB
        for (size_t i = 0; i < treeNode->getObjectList().size(); i++) {
            Intersection* intersection = treeNode->getObjectList()[i]->getObject()->intersect(ray);

            if (intersection != nullptr) {
                if (closestIntersection == nullptr) {
                    // If an intersection hasn't been found yet

                    // First check if the intersection is within the bounding box
                    if (treeNode->getAxisAlignedBoundingBox()->intersectionInsideBox(intersection)) {
                        closestIntersection = intersection;
                        closestObject = treeNode->getObjectList()[i]->getObject();
                    }
                } else {
                    // If an intersection was previously found
                    // Check if new intersection is closer and within box
                    if ((treeNode->getAxisAlignedBoundingBox()->intersectionInsideBox(intersection))
                        && (intersection->getIntersectionPoint().distance(&coordOrigin)
                            < closestIntersection->getIntersectionPoint().distance(&coordOrigin))) {
                        delete closestIntersection;
                        closestIntersection = intersection;
                        closestObject = treeNode->getObjectList()[i]->getObject();
                    } else {
                        delete intersection;
                    }
                }
            }
        }
        // At this point, closest intersection has been found OR there was no intersection
        // We proceed by applying a BRDF
        if (closestIntersection != nullptr) {
            intersectionFound = true;
            closestIntersection->setObject(closestObject);
            Vector viewingDirection = ray->getDirection();
            viewingDirection.normalize();
            viewingDirection.scale(-1);
            closestIntersection->setViewingDirection(viewingDirection);

            bool isShadow = false;

            // Looping through the rest of the object list to see if the point is illuminated
            // If not illuminated, point is 'in shadow'.
            // We want to loop through the 'model objects' otherwise this will take forever to
            // run.
            for (size_t i = 0; i < m_modelList.size(); i++) {
                // If the object isn't in the model

                if (closestObject->getModelAddress() != m_modelList[i]) {

                    // As of right now, only checks the first light source
                    LightSource* light = m_lightSourceList[0];
                    Vector sourceVector = Vector(
                        light->getPosition().getPoint() - closestIntersection->getIntersectionPoint().getPoint());
                    sourceVector.normalize();
                    Ray* lightSourceRay = new Ray(closestIntersection->getIntersectionPoint(), sourceVector);

                    for (size_t j = 0; j < m_modelList[i]->size(); j++) {
                        Intersection* lightObjectIntersection = m_modelList[i]->at(j)->intersect(lightSourceRay);

                        if (lightObjectIntersection != nullptr) {
                            isShadow = true;
                            delete lightObjectIntersection;
                            delete lightSourceRay;
                            goto objectInShadowKD;
                        }

                        delete lightObjectIntersection;
                    }

                    delete lightSourceRay;
                }
            }

        objectInShadowKD:
            if (!isShadow) {
                closestIntersection->setLightSources(m_lightSourceList);
            }

            closestObjectRadiance = closestObject->getIlluminationModel()->illuminate(closestIntersection);
            delete closestIntersection;
        }
        return FinalRadiance(closestObjectRadiance, intersectionFound);
    }

    // We get here if we're not a leaf
    float splitDistance = treeNode->getSplitDistance();
    int dimSplit = (treeNode->getDimSplit() == KdTreeNode::DimSplit::X)
        ? 0
        : ((treeNode->getDimSplit() == KdTreeNode::DimSplit::Y) ? 1 : 2);
    bool rayMovingPositive = (ray->getDirection().getVector()(dimSplit) > 0);
    AABBIntersection aabbIntersection
        = treeNode->getAxisAlignedBoundingBox()->intersectRay(ray, splitDistance, dimSplit);
    if (aabbIntersection.m_intersects == false) {
        return FinalRadiance(
            Radiance(BACKGROUND_RADIANCE_RED, BACKGROUND_RADIANCE_GREEN, BACKGROUND_RADIANCE_BLUE), false);
    } else {
        if (aabbIntersection.m_t <= aabbIntersection.m_tStart) {
            // Case 1: t <= tStart <= tEnd --> Cull front side
            if (rayMovingPositive) {
                return traverseKDTree(ray, treeNode->getBackNode());
            } else {
                return traverseKDTree(ray, treeNode->getFrontNode());
            }

        } else if (aabbIntersection.m_t >= aabbIntersection.m_tEnd) {
            // Case 2: tStart <= tEnd <= t --> Cull back side
            if (rayMovingPositive) {
                return traverseKDTree(ray, treeNode->getFrontNode());
            } else {
                return traverseKDTree(ray, treeNode->getBackNode());
            }
        } else {
            // Case 3: Traverse both sides in turn
            if (rayMovingPositive) {
                FinalRadiance frontTest = traverseKDTree(ray, treeNode->getFrontNode());
                if (frontTest.m_intersectionFound == true) {
                    // Early ray termination
                    return frontTest;
                }
                return traverseKDTree(ray, treeNode->getBackNode());
            } else {
                FinalRadiance frontTest = traverseKDTree(ray, treeNode->getBackNode());
                if (frontTest.m_intersectionFound == true) {
                    // Early ray termination
                    return frontTest;
                }
                return traverseKDTree(ray, treeNode->getFrontNode());
            }
        }
    }
}

Radiance World::spawnRay(Ray* ray)
{
    if (USE_KD_TREES) {
        return traverseKDTree(ray, m_sceneKDTree).m_radiance;

    } else {
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
            closestIntersection->setObject(closestObject);
            Vector viewingDirection = ray->getDirection();
            viewingDirection.normalize();
            viewingDirection.scale(-1);
            closestIntersection->setViewingDirection(viewingDirection);

            bool isShadow = false;

            // Looping through the rest of the object list to see if the point is illuminated
            // If not illuminated, point is 'in shadow'
            for (size_t i = 0; i < m_modelList.size(); i++) {
                // If the object isn't in the model
                if (closestObject->getModelAddress() != m_modelList[i]) {

                    // As of right now, only checks the first light source
                    LightSource* light = m_lightSourceList[0];
                    Vector sourceVector = Vector(
                        light->getPosition().getPoint() - closestIntersection->getIntersectionPoint().getPoint());
                    sourceVector.normalize();
                    Ray* lightSourceRay = new Ray(closestIntersection->getIntersectionPoint(), sourceVector);

                    for (size_t j = 0; j < m_modelList[i]->size(); j++) {
                        Intersection* lightObjectIntersection = m_modelList[i]->at(j)->intersect(lightSourceRay);

                        if (lightObjectIntersection != nullptr) {
                            isShadow = true;
                            delete lightObjectIntersection;
                            delete lightSourceRay;
                            goto objectInShadow;
                        }

                        delete lightObjectIntersection;
                    }

                    delete lightSourceRay;
                }
            }

        objectInShadow:
            if (!isShadow) {
                closestIntersection->setLightSources(m_lightSourceList);
            }

            closestObjectRadiance = closestObject->getIlluminationModel()->illuminate(closestIntersection);

            delete closestIntersection;
        }

        return closestObjectRadiance;
    }
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

void World::buildKDTree()
{
    KdTreeNode* sceneKDTree = new KdTreeNode();
    if (m_objectList.size() != 0) {
        AxisAlignedBoundingBox* aabb = m_objectList[0]->getAxisAlignedBoundingBox();

        float xMin, xMax, yMin, yMax, zMin, zMax;
        xMin = aabb->getXMin();
        xMax = aabb->getXMax();
        yMin = aabb->getYMin();
        yMax = aabb->getYMax();
        zMin = aabb->getZMin();
        zMax = aabb->getZMax();
        m_aabbList.push_back(aabb);

        for (size_t i = 1; i < m_objectList.size(); i++) {
            AxisAlignedBoundingBox* aabbox = m_objectList[i]->getAxisAlignedBoundingBox();
            if (aabbox->getXMin() < xMin) {
                xMin = aabbox->getXMin();
            }
            if (aabbox->getXMax() > xMax) {
                xMax = aabbox->getXMax();
            }
            if (aabbox->getYMin() < yMin) {
                yMin = aabbox->getYMin();
            }
            if (aabbox->getYMax() > yMax) {
                yMax = aabbox->getYMax();
            }
            if (aabbox->getZMin() < zMin) {
                zMin = aabbox->getZMin();
            }
            if (aabbox->getZMax() > zMax) {
                zMax = aabbox->getZMax();
            }
            m_aabbList.push_back(aabbox);
        }

        // aabb of entire scene
        AxisAlignedBoundingBox* sceneAABB = new AxisAlignedBoundingBox(xMin, xMax, yMin, yMax, zMin, zMax);

        sceneKDTree->setAxisAlignedBoundingBox(sceneAABB);
        sceneKDTree->setDimSplit(KdTreeNode::DimSplit::X);
        sceneKDTree->setSplitDistance((xMin + xMax) / 2.0f);

        if (USE_SURFACE_AREA_HEURISTIC) {
            sceneKDTree = getNodeSAH(sceneKDTree, m_aabbList);
        } else {
            sceneKDTree = getNode(sceneKDTree, m_aabbList);
        }

        std::cout << "Number of objects in scene: " << m_objectList.size() << std::endl;
        std::cout << "Size of KD Tree: " << sceneKDTree->getSize() << std::endl;
        // sceneKDTree->printTree();
    }
    m_sceneKDTree = sceneKDTree;
}

KdTreeNode* World::getNode(KdTreeNode* voxel, std::vector<AxisAlignedBoundingBox*> primitives, int recursiveLevel)
{
    AxisAlignedBoundingBox* aabb = voxel->getAxisAlignedBoundingBox();

    // termination
    if (TERMINATE_WITH_LEVELS) {
        if (recursiveLevel == 0) {
            voxel->setObjectList(primitives);
            voxel->setIsLeaf(true);
            return voxel;
        }
    } else if (TERMINATE_WITH_VOLUME) {
        if (aabb->getVolume() < VOXEL_TERMINAL_VOLUME) {
            voxel->setObjectList(primitives);
            voxel->setIsLeaf(true);
            return voxel;
        }
    } else if (TERMINATE_WITH_PRIMITIVES) {
        if (primitives.size() <= MAX_PRIMITIVES_PER_VOXEL) {
            voxel->setObjectList(primitives);
            voxel->setIsLeaf(true);
            return voxel;
        }
    }

    float partitioningPlane = voxel->getSplitDistance();
    KdTreeNode::DimSplit currentDim = voxel->getDimSplit();
    KdTreeNode::DimSplit nextDim = voxel->getNextDim();

    AxisAlignedBoundingBox* frontAABB;
    AxisAlignedBoundingBox* backAABB;

    std::vector<AxisAlignedBoundingBox*> frontPrimitives;
    std::vector<AxisAlignedBoundingBox*> backPrimitives;

    // Get front and back voxel
    if (currentDim == KdTreeNode::DimSplit::X) {
        frontAABB = new AxisAlignedBoundingBox(
            aabb->getXMin(), partitioningPlane, aabb->getYMin(), aabb->getYMax(), aabb->getZMin(), aabb->getZMax());
        backAABB = new AxisAlignedBoundingBox(
            partitioningPlane, aabb->getXMax(), aabb->getYMin(), aabb->getYMax(), aabb->getZMin(), aabb->getZMax());
    } else if (currentDim == KdTreeNode::DimSplit::Y) {
        frontAABB = new AxisAlignedBoundingBox(
            aabb->getXMin(), aabb->getXMax(), aabb->getYMin(), partitioningPlane, aabb->getZMin(), aabb->getZMax());
        backAABB = new AxisAlignedBoundingBox(
            aabb->getXMin(), aabb->getXMax(), partitioningPlane, aabb->getYMax(), aabb->getZMin(), aabb->getZMax());
    } else {
        frontAABB = new AxisAlignedBoundingBox(
            aabb->getXMin(), aabb->getXMax(), aabb->getYMin(), aabb->getYMax(), aabb->getZMin(), partitioningPlane);
        backAABB = new AxisAlignedBoundingBox(
            aabb->getXMin(), aabb->getXMax(), aabb->getYMin(), aabb->getYMax(), partitioningPlane, aabb->getZMax());
    }

    // Partition primitives
    for (size_t i = 0; i < primitives.size(); i++) {
        if (primitives[i]->intersect(frontAABB)) {
            frontPrimitives.push_back(primitives[i]);
        }

        if (primitives[i]->intersect(backAABB)) {
            backPrimitives.push_back(primitives[i]);
        }
    }

    KdTreeNode* front = new KdTreeNode();
    front->setDimSplit(nextDim);
    front->setAxisAlignedBoundingBox(frontAABB);
    if (nextDim == KdTreeNode::DimSplit::X) {
        front->setSplitDistance((frontAABB->getXMin() + frontAABB->getXMax()) / 2.0f);
    } else if (nextDim == KdTreeNode::DimSplit::Y) {
        front->setSplitDistance((frontAABB->getYMin() + frontAABB->getYMax()) / 2.0f);
    } else {
        front->setSplitDistance((frontAABB->getZMin() + frontAABB->getZMax()) / 2.0f);
    }
    front = getNode(front, frontPrimitives, recursiveLevel - 1);

    KdTreeNode* back = new KdTreeNode();
    back->setDimSplit(nextDim);
    back->setAxisAlignedBoundingBox(backAABB);
    if (nextDim == KdTreeNode::DimSplit::X) {
        back->setSplitDistance((backAABB->getXMin() + backAABB->getXMax()) / 2.0f);
    } else if (nextDim == KdTreeNode::DimSplit::Y) {
        back->setSplitDistance((backAABB->getYMin() + backAABB->getYMax()) / 2.0f);
    } else {
        back->setSplitDistance((backAABB->getZMin() + backAABB->getZMax()) / 2.0f);
    }
    back = getNode(back, backPrimitives, recursiveLevel - 1);

    voxel->setFrontNode(front);
    voxel->setBackNode(back);

    return voxel;
}

KdTreeNode* World::getNodeSAH(KdTreeNode* voxel, std::vector<AxisAlignedBoundingBox*>& primitives, int recursiveLevel)
{

    AxisAlignedBoundingBox* aabb = voxel->getAxisAlignedBoundingBox();

    // termination
    if (TERMINATE_WITH_LEVELS) {
        if (recursiveLevel == 0) {
            voxel->setObjectList(primitives);
            voxel->setIsLeaf(true);
            return voxel;
        }
    } else if (TERMINATE_WITH_VOLUME) {
        if (aabb->getVolume() < VOXEL_TERMINAL_VOLUME) {
            voxel->setObjectList(primitives);
            voxel->setIsLeaf(true);
            return voxel;
        }
    } else if (TERMINATE_WITH_PRIMITIVES) {
        if (primitives.size() <= MAX_PRIMITIVES_PER_VOXEL) {
            voxel->setObjectList(primitives);
            voxel->setIsLeaf(true);
            return voxel;
        }
    }

    // Initialize the split distance to 0 in the X-Direction
    float splitDistance = 0;
    KdTreeNode::DimSplit splitDimension = KdTreeNode::DimSplit::X;

    float minimumCost = std::numeric_limits<float>::max();

    // Sorting based on midpoint of AABB along certain axis

    // Sort by X
    std::stable_sort(primitives.begin(), primitives.end(),
        [](const auto& aabb1, const auto& aabb2) { return aabb1->getMidPoint(0) < aabb2->getMidPoint(0); });
    findOptimalSplit(primitives, splitDistance, splitDimension, minimumCost, KdTreeNode::DimSplit::X, aabb);

    // Sort by Y
    std::stable_sort(primitives.begin(), primitives.end(),
        [](const auto& aabb1, const auto& aabb2) { return aabb1->getMidPoint(1) < aabb2->getMidPoint(1); });
    findOptimalSplit(primitives, splitDistance, splitDimension, minimumCost, KdTreeNode::DimSplit::Y, aabb);

    // Sort by Z
    std::stable_sort(primitives.begin(), primitives.end(),
        [](const auto& aabb1, const auto& aabb2) { return aabb1->getMidPoint(2) < aabb2->getMidPoint(2); });
    findOptimalSplit(primitives, splitDistance, splitDimension, minimumCost, KdTreeNode::DimSplit::Z, aabb);

    // Assign values to current voxel
    voxel->setDimSplit(splitDimension);
    voxel->setSplitDistance(splitDistance);

    // Set up front and back AABB
    AxisAlignedBoundingBox* frontAABB = nullptr;
    AxisAlignedBoundingBox* backAABB = nullptr;

    std::vector<AxisAlignedBoundingBox*> frontPrimitives;
    std::vector<AxisAlignedBoundingBox*> backPrimitives;

    // Get front and back voxel
    if (splitDimension == KdTreeNode::DimSplit::X) {
        frontAABB = new AxisAlignedBoundingBox(
            aabb->getXMin(), splitDistance, aabb->getYMin(), aabb->getYMax(), aabb->getZMin(), aabb->getZMax());
        backAABB = new AxisAlignedBoundingBox(
            splitDistance, aabb->getXMax(), aabb->getYMin(), aabb->getYMax(), aabb->getZMin(), aabb->getZMax());
    } else if (splitDimension == KdTreeNode::DimSplit::Y) {
        frontAABB = new AxisAlignedBoundingBox(
            aabb->getXMin(), aabb->getXMax(), aabb->getYMin(), splitDistance, aabb->getZMin(), aabb->getZMax());
        backAABB = new AxisAlignedBoundingBox(
            aabb->getXMin(), aabb->getXMax(), splitDistance, aabb->getYMax(), aabb->getZMin(), aabb->getZMax());
    } else {
        frontAABB = new AxisAlignedBoundingBox(
            aabb->getXMin(), aabb->getXMax(), aabb->getYMin(), aabb->getYMax(), aabb->getZMin(), splitDistance);
        backAABB = new AxisAlignedBoundingBox(
            aabb->getXMin(), aabb->getXMax(), aabb->getYMin(), aabb->getYMax(), splitDistance, aabb->getZMax());
    }

    // Partition primitives
    for (size_t i = 0; i < primitives.size(); i++) {
        if (primitives[i]->intersect(frontAABB)) {
            frontPrimitives.push_back(primitives[i]);
        }

        if (primitives[i]->intersect(backAABB)) {
            backPrimitives.push_back(primitives[i]);
        }
    }

    KdTreeNode* front = new KdTreeNode();
    front->setAxisAlignedBoundingBox(frontAABB);
    front = getNodeSAH(front, frontPrimitives, recursiveLevel - 1);

    KdTreeNode* back = new KdTreeNode();
    back->setAxisAlignedBoundingBox(backAABB);
    back = getNodeSAH(back, backPrimitives, recursiveLevel - 1);

    voxel->setFrontNode(front);
    voxel->setBackNode(back);

    return voxel;
}

void World::findOptimalSplit(std::vector<AxisAlignedBoundingBox*>& sortedPrimitives, float& currentSplitDistance,
    KdTreeNode::DimSplit& currentDimension, float& currentCost, KdTreeNode::DimSplit splitDimension,
    AxisAlignedBoundingBox* currentVoxel)
{
    int direction
        = (splitDimension == KdTreeNode::DimSplit::X ? 0 : (splitDimension == KdTreeNode::DimSplit::Y ? 1 : 2));
    for (size_t i = 0; i < sortedPrimitives.size(); i++) {
        // We choose upper/lower bounds of each object's AABB as the possible valid split planes
        float minBound = sortedPrimitives[i]->getMinInDirection(direction);
        float maxBound = sortedPrimitives[i]->getMaxInDirection(direction);

        // float costMinBound
        //     = computeCost(sortedPrimitives, direction, minBound, currentVoxel->getSurfaceArea(), currentVoxel);

        // if (costMinBound < currentCost) {
        //     currentCost = costMinBound;
        //     currentDimension = splitDimension;
        //     currentSplitDistance = minBound;
        // }

        float costMaxBound
            = computeCost(sortedPrimitives, direction, maxBound, currentVoxel->getSurfaceArea(), currentVoxel);

        if (costMaxBound < currentCost) {
            currentCost = costMaxBound;
            currentDimension = splitDimension;
            currentSplitDistance = maxBound;
        }
    }
}

float World::computeCost(std::vector<AxisAlignedBoundingBox*>& sortedPrimitives, int direction, float splitDistance,
    float parentSurfaceArea, AxisAlignedBoundingBox* currentVoxel)
{
    int numInLeftNode = 0;
    size_t iterator = 0;
    bool leftNodeDone = false;

    while (!leftNodeDone && iterator < sortedPrimitives.size()) {
        float midPoint = sortedPrimitives[iterator]->getMidPoint(direction);
        if (midPoint <= splitDistance) {
            numInLeftNode++;
        } else {
            leftNodeDone = true;
        }
        iterator++;
    }

    int numInRightNode = sortedPrimitives.size() - numInLeftNode;

    float leftNodeSurfaceArea = 0.0f;
    float rightNodeSurfaceArea = 0.0f;

    // Get front and back voxel
    if (direction == 0) {
        leftNodeSurfaceArea = getSurfaceArea(currentVoxel->getXMin(), splitDistance, currentVoxel->getYMin(),
            currentVoxel->getYMax(), currentVoxel->getZMin(), currentVoxel->getZMax());
        rightNodeSurfaceArea = getSurfaceArea(splitDistance, currentVoxel->getXMax(), currentVoxel->getYMin(),
            currentVoxel->getYMax(), currentVoxel->getZMin(), currentVoxel->getZMax());
    } else if (direction == 1) {
        leftNodeSurfaceArea = getSurfaceArea(currentVoxel->getXMin(), currentVoxel->getXMax(), currentVoxel->getYMin(),
            splitDistance, currentVoxel->getZMin(), currentVoxel->getZMax());
        rightNodeSurfaceArea = getSurfaceArea(currentVoxel->getXMin(), currentVoxel->getXMax(), splitDistance,
            currentVoxel->getYMax(), currentVoxel->getZMin(), currentVoxel->getZMax());
    } else {
        leftNodeSurfaceArea = getSurfaceArea(currentVoxel->getXMin(), currentVoxel->getXMax(), currentVoxel->getYMin(),
            currentVoxel->getYMax(), currentVoxel->getZMin(), splitDistance);
        rightNodeSurfaceArea = getSurfaceArea(currentVoxel->getXMin(), currentVoxel->getXMax(), currentVoxel->getYMin(),
            currentVoxel->getYMax(), splitDistance, currentVoxel->getZMax());
    }

    return COST_OF_TRAVERSAL + numInLeftNode * leftNodeSurfaceArea / parentSurfaceArea
        + numInRightNode * rightNodeSurfaceArea / parentSurfaceArea;
}

float World::getSurfaceArea(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
    float width = xMax - xMin;
    float height = yMax - yMin;
    float depth = zMax - zMin;
    return 2.0f * (width * height + height * depth + depth * width);
}