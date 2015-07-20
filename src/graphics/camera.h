
#ifndef SRC_GRAPHICS_CAMERA_H_
#define SRC_GRAPHICS_CAMERA_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Camera(float xPosition, float yPosition, float zPosition);
	virtual ~Camera();

// ########################################################
// Member Functions########################################
// ########################################################

	glm::mat4 getViewMatrix();

	void moveForward(float amount);

	void changeViewDirection(float xOffset, float yOffset);

	void setLocation(float x, float y, float z);

// ########################################################
// Instance Variables #####################################
// ########################################################

private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
	float horizontalAngle = 3.0;
	float verticalAngle = 0.0f;

    // Camera options
    GLfloat inputSensX = 0.01;
    GLfloat inputSensY = 0.01;

    float maxVerticalAngle = 3.14/2;
    float minVerticalAngle = -3.14/2;
};

#endif /* SRC_GRAPHICS_CAMERA_H_ */
