#ifndef ILLUMINATION_MODEL_H
#define ILLUMINATION_MODEL_H

/**
 * @brief Illumination Model.
 *
 * Represents an Illumination Model.
 */
class IlluminationModel {
public:
    virtual void illuminate() = 0;
    virtual ~IlluminationModel() { };
};

#endif