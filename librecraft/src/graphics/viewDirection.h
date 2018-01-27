#ifndef SRC_GRAPHICS_VIEWDIRECTION_H_
#define SRC_GRAPHICS_VIEWDIRECTION_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../config/data.h"

namespace graphics {

/**
 *  \brief This class represents view direction vector.
 *
 *  The user can set and change the view direction angles with the
 *  changeViewDirection and serViedDirection functions. From this information
 *  this class is responsible to compute the vectors representing those angles.
 */
class ViewDirection {
public:
  /**
   * \brief Increases the horizontal and vertical angles with the provided
   *        amount.
   *
   * @param horizontal The amount that the horizontal angle should be increased.
   * @param vertical The amount that the vertical angle shoud be increased.
   */
  void changeViewDirection(float horizontal, float vertical);

  /**
   * \brief Set the horizontal and vertical angles.
   *
   * @param horizontal The new horizontal angle.
   * @param vertical The new vertical angle.
   */
  void setViewDirection(float horizontal, float vertical);

  /**
   *
   * @return The view direction representing the view angles.
   */
  glm::vec3 getViewDirection();

  /**
   * \brief Returns the vector that is considered upwards and with a 90* angle
   *        to the viewDirection.
   *
   * @return The direction pointing upwards.
   */
  glm::vec3 getUpDirection();

  /**
   * \brief Returns the vector that is considered right and with a 90* angle to
   * the viewDirection.
   *
   * @return The direction pointing to the right.
   */
  glm::vec3 getRightDirection();

  /**
   * \brief Sets the sensitivity used to as a multiplier when changing the view
   * direction in changeViewDirection.
   *
   * @param x The new sensitivity for the horizontal changes.
   * @param y The new sensitivity for the vertical changes.
   */
  void setSensitivity(double x, double y);

private:
  double m_sensX{config::input_data::mouseSensitivityX};
  double m_sensY{config::input_data::mouseSensitivityY};

  glm::vec3 m_direction;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_worldUp{0.0f, 1.0f, 0.0f};

  double m_horizontalAngle{0.1f};
  double m_verticalAngle{0.0f};

  double m_maxVerticalAngle{3.14 / 2.0};
  double m_minVerticalAngle{-3.14 / 2.0};
};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_VIEWDIRECTION_H_ */
