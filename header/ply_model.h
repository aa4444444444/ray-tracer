#ifndef PLY_MODEL_H
#define PLY_MODEL_H

#include "triangle.h"
#include <Eigen/Dense>
#include <vector>

class World;

/**
 * @brief Represents an model defined by a .ply file
 *
 */
class PlyModel {
public:
    // Automatically adds them to the world to avoid an extra for loop later
    PlyModel(std::vector<float> vertexX, std::vector<float> vertexY, std::vector<float> vertexZ,
        std::vector<std::vector<int>> indices, Color c, float scaleAmount, const Eigen::Vector3d& rotateVec,
        const Eigen::Vector3d& translateVec);
    std::vector<Object*> getPrimitives();

private:
    std::vector<Object*> m_primitives;
};

#endif