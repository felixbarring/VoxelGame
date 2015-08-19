
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

Camera::~Camera()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

glm::mat4 Camera::getViewMatrix()
{
	// TODO Consider cache the matrix if it is called many times in one frame!
	return glm::lookAt(position, position+direction, up);
}

void Camera::moveForward(float amount)
{
	position += direction * amount;
}

void Camera::setViewDirection(glm::vec3 direction)
{
	this->direction = direction;
}

void Camera::setViewDirection(float x, float y, float z)
{
	this->direction = glm::vec3(x, y, z);
}


void Camera::setUpDirection(glm::vec3 up)
{
	this->up = up;
}

void Camera::setUpDirection(float x, float y, float z)
{
	this->up = glm::vec3(x, y, z);
}

void Camera::setLocation(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

}

