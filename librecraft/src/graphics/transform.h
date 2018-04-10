#ifndef SRC_GRAPHICS_TRANSFORM_H_
#define SRC_GRAPHICS_TRANSFORM_H_

#include <glm/glm.hpp>

namespace graphics {

// TODO Add glm::vec3 input.

/**
 * @brief A convenience class that handles the transform matrix for a graphical
 * object the position and rotation around x, y, and z axles.
 */
class Transform {
public:
  /**
   * @brief Creates a transform matrix at the the specified location with no
   * rotation.
   *
   * @param x The x location for the transform.
   * @param y The y location for the transform.
   * @param z The z location for the transform.
   */
  Transform(double x, double y, double z);

  /**
   * @brief Translated the matrix with the specified values.
   *
   * @param translation The change in translation for the matrix.
   */
  void translate(glm::vec3 translation);

  /**
   * @brief Translated the matrix with the specified values.
   *
   * @param x The x value for the translation change.
   * @param y The y value for the translation change.
   * @param z The z value for the translation change.
   */
  void translate(double x, double y, double z);

  // Make one that takes glm::vec3 ?!?
  /**
   * @brief Sets the translation for the matrix
   *
   * @param x The new translation x value.
   * @param y The new translation y value.
   * @param z The new translation z value.
   */
  void setLocation(double x, double y, double z);

  /**
   * @brief Sets the rotation around the x axis.
   *
   * @param value The angle rotation around the x axis.
   */
  void setRotationX(double value);

  /**
   * @brief Sets the rotation around the y axis.
   *
   * @param value The angle rotation around the y axis.
   */
  void setRotationY(double value);

  /**
   * '@brief Sets the rotation around the z axis.
   *
   * @param value The angle rotation around the z axis.
   */
  void setRotationZ(double value);

  /**
   * @brief Rotates around the x axis.
   *
   * @param value The amount that the rotation around the x axis should
   * increase.
   */
  void rotateX(double value);

  /**
   * @brief Rotates around the y axis.
   *
   * @param value The amount that the rotation around the y axis should
   * increase-
   */
  void rotateY(double value);

  /**
   * @brief Rotates around the z axis.
   *
   * @param value The amount that the rotation around the z axis should
   * increase.
   */
  void rotateZ(double value);

  /**
   * @brief Returns the matrix that represents the transform.
   *
   * The transform can be used to translate graphical objects into world space.
   *
   * @return The transform matrix.
   */
  glm::mat4 getMatrix();

private:
  glm::mat4 translation;
  glm::mat4 xRotation;
  glm::mat4 yRotation;
  glm::mat4 zRotation;
};
}

#endif /* SRC_GRAPHICS_TRANSFORM_H_ */
