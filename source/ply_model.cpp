#include "../header/ply_model.h"
#include "../header/phong.h"
#include "../header/world.h"

// TODO: Give user the ability to customize the BRDF and stuff
PlyModel::PlyModel(std::vector<float> vertexX, std::vector<float> vertexY, std::vector<float> vertexZ,
    std::vector<std::vector<int>> indices, Color c, float scaleAmount, const Eigen::Vector3d& translateVec)
{
    for (size_t i = 0; i < indices.size(); i++) {
        int triangleIndex1 = indices[i][0];
        int triangleIndex2 = indices[i][1];
        int triangleIndex3 = indices[i][2];

        // Basic Phong-Blinn Shading
        Phong* phong = new Phong(0.9, 0.3, 0.65, 50);
        phong->setPhongBlinn(true);

        Triangle* triangle = new Triangle(vertexX[triangleIndex1], vertexY[triangleIndex1], vertexZ[triangleIndex1],
            vertexX[triangleIndex2], vertexY[triangleIndex2], vertexZ[triangleIndex2], vertexX[triangleIndex3],
            vertexY[triangleIndex3], vertexZ[triangleIndex3]);
        triangle->setColor(c);
        triangle->setIlluminationModel(phong);
        triangle->scale(scaleAmount);
        triangle->translate(translateVec);

        m_primitives.push_back(triangle);
    }
}

std::vector<Object*> PlyModel::getPrimitives() { return m_primitives; }