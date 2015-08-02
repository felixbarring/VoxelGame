
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

	ViewDirection();
	virtual ~ViewDirection();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	void changeViewDirection(float horizontal, float vertical);

	void setViewDirection(float horizontal, float vertical);

	glm::vec3 getViewDirection();

	glm::vec3 getUpDirection();

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

private:

	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
	float horizontalAngle = 0.1f;
	float verticalAngle = 0.0f;

	float maxVerticalAngle = 3.14/2;
	float minVerticalAngle = -3.14/2;

	float xSensitivity = 0.01;
	float ySensitivity = 0.01;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_VIEWDIRECTION_H_ */
