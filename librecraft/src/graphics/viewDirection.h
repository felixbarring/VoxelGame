#ifndef SRC_GRAPHICS_VIEWDIRECTION_H_
#define SRC_GRAPHICS_VIEWDIRECTION_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace graphics {

class ViewDirection {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	ViewDirection() {
	}
	;

	virtual ~ViewDirection() {
	}
	;

	// ########################################################
	// Member Functions########################################
	// ########################################################

	void changeViewDirection(float horizontal, float vertical);

	void setViewDirection(float horizontal, float vertical);

	glm::vec3 getViewDirection();

	glm::vec3 getUpDirection();

	glm::vec3 getRightDirection();

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	glm::vec3 m_direction;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp { 0.0f, 1.0f, 0.0f };
	float m_horizontalAngle = 0.1f;
	float m_verticalAngle = 0.0f;

	float m_maxVerticalAngle = 3.14 / 2;
	float m_minVerticalAngle = -3.14 / 2;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_VIEWDIRECTION_H_ */
