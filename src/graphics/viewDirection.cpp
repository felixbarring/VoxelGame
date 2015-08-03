
#include "viewDirection.h"

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ViewDirection::ViewDirection()
{
}

ViewDirection::~ViewDirection()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void ViewDirection::changeViewDirection(float horizontal, float vertical)
{
	ViewDirection::setViewDirection(horizontalAngle + horizontal * xSensitivity, verticalAngle + vertical * ySensitivity);
}

void ViewDirection::setViewDirection(float horizontal, float vertical)
{
	horizontalAngle = horizontal;
	verticalAngle = vertical;

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

	right = glm::normalize(glm::cross(direction, worldUp));
	up = glm::normalize(glm::cross(direction, right));
}

glm::vec3 ViewDirection::getViewDirection()
{
	return direction;
}

glm::vec3 ViewDirection::getUpDirection()
{
	return up;
}

glm::vec3 ViewDirection::getRightDirection()
{
	return right;
}

} /* namespace graphics */