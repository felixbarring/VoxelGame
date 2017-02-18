#include "camera.h"

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Camera::Camera()
    : m_position{0.0, 0.0, 0.0}
{
    setFov(config::graphics_data::fov);
}

Camera::Camera(float xPosition, float yPosition, float zPosition)
    : m_position{xPosition, yPosition, zPosition}
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void Camera::setViewMatrix(glm::mat4 value) {
    this->m_view = value;
}

glm::mat4 Camera::getViewMatrix() {
    return m_view;
}

void Camera::setProjectionMatrix(glm::mat4 value) {
    m_projection = value;
}

glm::mat4& Camera::getProjectionMatrix() {
    return m_projection;
}

glm::vec3& Camera::getPosition() {
    return m_position;
}

void Camera::updateView(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
    this->m_position = position;
    m_view = glm::lookAt(position, position + direction, up);
}

void Camera::setFov(float value) {
    static const float degreeToRadian{(3.14 / 180)};
    m_projection = glm::perspective(value * degreeToRadian, m_aspectRatio, m_close, m_far);
}

}

