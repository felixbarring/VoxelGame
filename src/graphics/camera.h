
#ifndef SRC_GRAPHICS_CAMERA_H_
#define SRC_GRAPHICS_CAMERA_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../config/data.h"

class Camera {
private:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Camera(float xPosition, float yPosition, float zPosition);
	virtual ~Camera();

// ########################################################
// Member Functions########################################
// ########################################################

public:

	static Camera& getInstance()
	{
		static Camera INSTANCE{0,0,0};
		return INSTANCE;
	}

	glm::mat4 getViewMatrix();

	glm::mat4& getProjectionMatrix() { return projection; }

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

	// TODO Make it possible to change stuff, like fov
	float aspectRatio = graphics_data::windowWidth / graphics_data::windowHeight;
	glm::mat4 projection = glm::perspective(graphics_data::fov, aspectRatio, 0.1f, 100.0f);

    // Camera options
    GLfloat inputSensX = 0.01;
    GLfloat inputSensY = 0.01;

    float maxVerticalAngle = 3.14/2;
    float minVerticalAngle = -3.14/2;
};

#endif /* SRC_GRAPHICS_CAMERA_H_ */
