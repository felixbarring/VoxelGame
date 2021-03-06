#include "viewDirection.h"

using namespace config;

namespace graphics {

void
ViewDirection::changeViewDirection(float horizontal, float vertical) {
  setViewDirection(m_horizontalAngle + horizontal * m_sensX,
                   m_verticalAngle + vertical * m_sensY);
}

void
ViewDirection::setViewDirection(float horizontal, float vertical) {
  m_horizontalAngle = horizontal;
  m_verticalAngle = vertical;

  if (m_verticalAngle > m_maxVerticalAngle) {
    m_verticalAngle = m_maxVerticalAngle;
  } else if (m_verticalAngle < m_minVerticalAngle) {
    m_verticalAngle = m_minVerticalAngle;
  }

  m_direction = glm::vec3{cos(m_verticalAngle) * sin(m_horizontalAngle),
                          sin(m_verticalAngle),
                          cos(m_verticalAngle) * cos(m_horizontalAngle)};

  m_right = glm::normalize(glm::cross(m_direction, m_worldUp));
  // Hack :o
  m_up = -glm::normalize(glm::cross(m_direction, m_right));
}

glm::vec3
ViewDirection::getViewDirection() {
  return m_direction;
}

glm::vec3
ViewDirection::getUpDirection() {
  return m_up;
}

glm::vec3
ViewDirection::getRightDirection() {
  return m_right;
}

void
ViewDirection::setSensitivity(double x, double y) {
  m_sensX = x;
  m_sensY = y;
}

} /* namespace graphics */
