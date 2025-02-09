#include "../header/phong.h"
#include "../header/util.h"
#include <iostream>

Phong::Phong(float k_a, float k_d, float k_s, float k_e)
{
    m_ka = k_a;
    m_kd = k_d;
    m_ks = k_s;
    m_ke = k_e;
}

Radiance Phong::illuminateInShadow(Color objectColor)
{
    // Just Ambient Component
    float radianceComponentRed = m_ka * objectColor.getRed() * BACKGROUND_RADIANCE_RED;
    float radianceComponentGreen = m_ka * objectColor.getGreen() * BACKGROUND_RADIANCE_GREEN;
    float radianceComponentBlue = m_ka * objectColor.getBlue() * BACKGROUND_RADIANCE_BLUE;

    return Radiance(radianceComponentRed, radianceComponentGreen, radianceComponentBlue);
}

Radiance Phong::illuminate(
    Color objectColor, Color specColor, Intersection* intersection, std::vector<LightSource*> lightSources)
{

    // std::cout << objectColor.getRed255() << " " << objectColor.getGreen255() << " " << objectColor.getBlue255()
    //           << std::endl;

    // Ambient Component
    float radianceComponentRed = m_ka * objectColor.getRed() * BACKGROUND_RADIANCE_RED;
    float radianceComponentGreen = m_ka * objectColor.getGreen() * BACKGROUND_RADIANCE_GREEN;
    float radianceComponentBlue = m_ka * objectColor.getBlue() * BACKGROUND_RADIANCE_BLUE;

    // Diffuse & Specular component
    for (size_t i = 0; i < lightSources.size(); i++) {
        LightSource* light = lightSources[i];
        float lightRadianceRed = light->getRadiance().getRadianceRed();
        float lightRadianceGreen = light->getRadiance().getRadianceGreen();
        float lightRadianceBlue = light->getRadiance().getRadianceBlue();

        Vector sourceVector = Vector(intersection->getIntersectionPoint().getPoint() - light->getPosition().getPoint());
        sourceVector.normalize();

        Vector reflection = findReflection(sourceVector, intersection->getNormal());
        sourceVector.scale(-1);
        Vector normal = intersection->getNormal();
        Vector view = intersection->getViewingDirection();

        float sourceDotNormal = std::max(0.0f, sourceVector.dot(&normal));
        float reflectionDotView = pow(std::max(0.0f, reflection.dot(&view)), m_ke);

        // Diffuse
        radianceComponentRed += m_kd * lightRadianceRed * objectColor.getRed() * sourceDotNormal;
        radianceComponentGreen += m_kd * lightRadianceGreen * objectColor.getGreen() * sourceDotNormal;
        radianceComponentBlue += m_kd * lightRadianceBlue * objectColor.getBlue() * sourceDotNormal;

        // Specular
        radianceComponentRed += m_ks * lightRadianceRed * specColor.getRed() * reflectionDotView;
        radianceComponentGreen += m_ks * lightRadianceGreen * specColor.getGreen() * reflectionDotView;
        radianceComponentBlue += m_ks * lightRadianceBlue * specColor.getBlue() * reflectionDotView;
    }

    return Radiance(radianceComponentRed, radianceComponentGreen, radianceComponentBlue);
}