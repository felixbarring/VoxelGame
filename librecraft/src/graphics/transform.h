#ifndef SRC_GRAPHICS_TRANSFORM_H_
#define SRC_GRAPHICS_TRANSFORM_H_

#include <glm/glm.hpp>

namespace graphics {

/**
 * \brief A convenience class that handles the transform matrix for a graphical object the position and rotation
 *        around x, y, and z axles.
 */
class Transform {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    /**
     * \brief Creates a transform matrix at the the specified location with no rotation.
     *
     * @param x The x location for the transform.
     * @param y The y location for the transform.
     * @param z The z location for the transform.
     */
    Transform(float x, float y, float z);

// ########################################################
// Member Functions########################################
// ########################################################

    /**
     * \brief Translated the matrix with the specified values.
     *
     * @param translation The change in translation for the matrix.
     */
    void translate(glm::vec3 translation);

    /**
     * \brief Translated the matrix with the specified values.
     *
     * @param x The x value for the translation change.
     * @param y The y value for the translation change.
     * @param z The z value for the translation change.
     */
    void translate(float x, float y, float z);

    // Make one that takes glm::vec3 ?!?
    /**
     * \brief Sets the translation for the matrix
     *
     * @param x The new translation x value.
     * @param y The new translation y value.
     * @param z The new translation z value.
     */
    void setLocation(float x, float y, float z);

    /**
     * \brief Sets the rotation around the x axis.
     *
     * @param value The angle rotation around the x axis.
     */
    void setRotationX(float value);

    /**
     * \brief Sets the rotation around the y axis.
     *
     * @param value The angle rotation around the y axis.
     */
    void setRotationY(float value);

    /**
     * '\brief Sets the rotation around the z axis.
     *
     * @param value The angle rotation around the z axis.
     */
    void setRotationZ(float value);

    /**
     * \brief Rotates around the x axis.
     *
     * @param value The amount that the rotation around the x axis should increase.
     */
    void rotateX(float value);

    /**
     * \brief Rotates around the y axis.
     *
     * @param value The amount that the rotation around the y axis should increase-
     */
    void rotateY(float value);

    /**
     * \brief Rotates around the z axis.
     *
     * @param value The amount that the rotation around the z axis should increase.
     */
    void rotateZ(float value);

    /**
     * \brief Returns the matrix that represents the transform.
     *
     * The transform can be used to translate graphical objects into world space.
     *
     * @return The transform matrix.
     */
    glm::mat4 getMatrix();

// ########################################################
// Implementation #########################################
// ########################################################

private:
    glm::mat4 translation;
    glm::mat4 xRotation;
    glm::mat4 yRotation;
    glm::mat4 zRotation;
};

}

#endif /* SRC_GRAPHICS_TRANSFORM_H_ */
