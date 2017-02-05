#include "camera.h"

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Camera::Camera()
    : position{0.0, 0.0, 0.0}
{
    setFov(config::graphics_data::fov);
}

Camera::Camera(float xPosition, float yPosition, float zPosition)
    : position{xPosition, yPosition, zPosition}
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void Camera::setViewMatrix(glm::mat4 view) {
    this->view = view;
}

glm::mat4 Camera::getViewMatrix() {
    return view;
}

void Camera::setProjectionMatrix(glm::mat4 view) {
    projection = view;
}

glm::mat4& Camera::getProjectionMatrix() {
    return projection;
}

glm::vec3& Camera::getPosition() {
    return position;
}

void Camera::updateView(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
    this->position = position;
    view = glm::lookAt(position, position + direction, up);
}

void Camera::setFov(float value) {
    static const float degreeToRadian{(3.14 / 180)};
    float fov = value * degreeToRadian;
    projection = glm::perspective(fov, aspectRatio, close, far);
}

}

