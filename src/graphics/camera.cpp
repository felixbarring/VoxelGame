
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
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Up vector
	glm::vec3 right = glm::normalize(glm::cross(worldUp, direction));
	glm::vec3 up = glm::normalize(glm::cross(right, direction));

	return glm::lookAt(position, position+direction, up);
}

void Camera::moveForward(float amount)
{
	position += direction * amount;
}

void Camera::changeViewDirection(float x, float y)
{
	horizontalAngle += inputSensX * x;
	verticalAngle += inputSensY * y;

	if (verticalAngle > maxVerticalAngle) {
		verticalAngle = maxVerticalAngle;
	} else if (verticalAngle < minVerticalAngle) {
		verticalAngle = minVerticalAngle;
	}

	direction = glm::vec3{
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	};

	glm::vec3 right = glm::normalize(glm::cross(worldUp, direction));
	glm::vec3 up = glm::normalize(glm::cross(right, direction));
}

void Camera::setLocation(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

}

