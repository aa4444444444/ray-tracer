#include "../header/ashikhmin_shirley.h"
#include "../header/constants.h"
#include "../header/object.h"
#include <algorithm>
#include <cmath>

AshikhminShirley::AshikhminShirley(float n_u, float n_v, float r_s, float r_d)
{
    m_nu = n_u;
    m_nv = n_v;
    m_rs = r_s;
    m_rd = r_d;
}

bool isClose(float a, float b, float tol = 1e-5f) { return std::fabs(a - b) < tol; }

bool areParallel(Vector A, Vector B, float tol = 1e-5f)
{
    float dot = A.dot(&B);

    // Check if the absolute dot product is close to 1 (parallel)
    return isClose(std::fabs(dot), 1.0f, tol);
}

float AshikhminShirley::schlickApprox(float kDotH) { return m_rs + (1 - m_rs) * pow((1 - kDotH), 5); }

Radiance AshikhminShirley::illuminate(Intersection* intersection)
{

    Color objectColor = intersection->getObject()->getColor();
    Color specColor = intersection->getObject()->getSpecColor();

    float diffuseFraction1 = (28 * m_rd * (1 - 0.5 * m_rs)) / (23 * PI);

    float specularFraction1 = sqrt((m_nu + 1) * (m_nv + 1)) / (8 * PI);

    float radianceComponentRed = 0.0f;
    float radianceComponentGreen = 0.0f;
    float radianceComponentBlue = 0.0f;

    std::vector<LightSource*> lightSources = intersection->getLightSources();

    // Specular and Diffuse Components
    for (size_t i = 0; i < lightSources.size(); i++) {
        LightSource* light = lightSources[i];
        if (!light->getIsInShadow()) {
            // normal = n
            Vector normal = intersection->getNormal();
            // view = k2
            Vector view = intersection->getViewingDirection();
            // source = k1
            Vector sourceVector
                = Vector(light->getPosition().getPoint() - intersection->getIntersectionPoint().getPoint());
            sourceVector.normalize();

            // 'k' vector in equation
            Vector k = Vector(view.getVector());
            float normalDotK1 = normal.dot(&sourceVector);
            float normalDotK2 = normal.dot(&view);

            if (normalDotK1 >= normalDotK2) {
                k = Vector(sourceVector.getVector());
            }

            Vector halfVector = Vector(sourceVector.getVector() + view.getVector());
            halfVector.normalize();

            Vector randomVector = Vector(1.0f, 0.0f, 0.0f);
            if (!areParallel(normal, randomVector)) {
                randomVector = Vector(0.0f, 1.0f, 0.0f);
            }

            // guaranteed to be on the plane
            Vector u = normal.cross(&randomVector);
            u.normalize();

            Vector v = normal.cross(&u);
            v.normalize();

            float specularExponent = (m_nu * pow(halfVector.dot(&u), 2) + m_nv * pow(halfVector.dot(&v), 2))
                / (1 - pow(normal.dot(&halfVector), 2));
            float specularFraction2Numerator = pow(normal.dot(&halfVector), specularExponent);
            float specularFraction2Denominator = halfVector.dot(&k) * std::max(normalDotK1, normalDotK2);
            float specularComponent = std::max(0.0f,
                (specularFraction1 * (specularFraction2Numerator / specularFraction2Denominator)
                    * schlickApprox(halfVector.dot(&k))));

            float diffuseComponent = std::max(0.0,
                (diffuseFraction1 * (1 - pow((1 - (normalDotK1 / 2.0f)), 5))
                    * (1 - pow((1 - (normalDotK2 / 2.0f)), 5))));

            radianceComponentRed += (light->getRadiance().getRadianceRed()
                    * (objectColor.getRed() * diffuseComponent + specColor.getRed() * specularComponent)
                + 0.2 * objectColor.getRed() * BACKGROUND_RADIANCE_RED);
            radianceComponentGreen += (light->getRadiance().getRadianceGreen()
                    * (objectColor.getGreen() * diffuseComponent + specColor.getGreen() * specularComponent)
                + 0.2 * objectColor.getGreen() * BACKGROUND_RADIANCE_GREEN);
            radianceComponentBlue += (light->getRadiance().getRadianceBlue()
                    * (objectColor.getBlue() * diffuseComponent + specColor.getBlue() * specularComponent)
                + 0.2 * objectColor.getBlue() * BACKGROUND_RADIANCE_BLUE);
        }
        // Reset light once we're done
        light->setIsInShadow(false);
    }

    return Radiance(radianceComponentRed, radianceComponentGreen, radianceComponentBlue);
}