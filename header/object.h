#ifndef OBJECT_H
#define OBJECT_H

#include "color.h"
#include "illumination_model.h"
#include "intersection.h"
#include "ray.h"
#include "texture.h"
#include <string>

/**
 * @brief Parent class for renderable objects.
 */
class Object {
public:
    virtual Intersection* intersect(Ray* ray) = 0;
    virtual void transform(Eigen::Matrix4d transMat) = 0;
    virtual Eigen::Vector2d getTextureUV(Point intersectionPoint) = 0;

    Color getColor() { return m_color; }
    void setColor(Color c)
    {
        m_color.setRed(c.getRed());
        m_color.setGreen(c.getGreen());
        m_color.setBlue(c.getBlue());
    }
    Color getSpecColor() { return m_specColor; }
    void setSpecColor(Color c)
    {
        m_specColor.setRed(c.getRed());
        m_specColor.setGreen(c.getGreen());
        m_specColor.setBlue(c.getBlue());
    }
    IlluminationModel* getIlluminationModel() { return m_illuminationModel; }
    void setIlluminationModel(IlluminationModel* illuminationModel) { m_illuminationModel = illuminationModel; }
    Texture* getTexture() { return m_texture; }
    void setTexture(Texture* texture) { m_texture = texture; }
    virtual ~Object()
    {
        if (m_illuminationModel != nullptr) {
            delete m_illuminationModel;
        }

        if (m_texture != nullptr) {
            delete m_texture;
        }
    };

protected:
    // Ambient/Diffuse color
    Color m_color;

    // Specular color
    Color m_specColor;

    // BRDF
    IlluminationModel* m_illuminationModel;

    // Texture
    Texture* m_texture;

private:
    std::string material;
};

#endif