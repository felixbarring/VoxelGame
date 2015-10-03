
#include "camera.h"

namespace graphics
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Camera::Camera(float xPosition, float yPosition, float zPosition):
	position{xPosition, yPosition, zPosition}
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void Camera::setViewMatrix(glm::mat4 view)
{
	this->view = view;
}

glm::mat4 Camera::getViewMatrix()
{
	return view;
}

glm::mat4& Camera::getProjectionMatrix()
{
	return projection;
}

void Camera::updateView(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
	view = glm::lookAt(position, position+direction, up);
}


}

