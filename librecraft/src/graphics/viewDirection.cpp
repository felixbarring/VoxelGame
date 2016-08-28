#include "viewDirection.h"

#include "../config/data.h"

using namespace config;

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

void ViewDirection::changeViewDirection(float horizontal, float vertical) {
	ViewDirection::setViewDirection(m_horizontalAngle + horizontal * config::input_data::mouseSensitivityX,
			m_verticalAngle + vertical * config::input_data::mouseSensitivityY);
}

void ViewDirection::setViewDirection(float horizontal, float vertical) {
	m_horizontalAngle = horizontal;
	m_verticalAngle = vertical;

	if (m_verticalAngle > m_maxVerticalAngle) {
		m_verticalAngle = m_maxVerticalAngle;
	} else if (m_verticalAngle < m_minVerticalAngle) {
		m_verticalAngle = m_minVerticalAngle;
	}

	m_direction = glm::vec3 { cos(m_verticalAngle) * sin(m_horizontalAngle),
							sin(m_verticalAngle),
							cos(m_verticalAngle) * cos(m_horizontalAngle) };

	m_right = glm::normalize(glm::cross(m_direction, m_worldUp));
	// Hack :o
	m_up = -glm::normalize(glm::cross(m_direction, m_right));
}

glm::vec3 ViewDirection::getViewDirection() {
	return m_direction;
}

glm::vec3 ViewDirection::getUpDirection() {
	return m_up;
}

glm::vec3 ViewDirection::getRightDirection() {
	return m_right;
}

} /* namespace graphics */
