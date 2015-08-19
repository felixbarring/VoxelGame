
#ifndef SRC_GRAPHICS_CAMERA_H_
#define SRC_GRAPHICS_CAMERA_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../config/data.h"

namespace graphics
{

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

	void setViewDirection(glm::vec3 direction);

	void setViewDirection(float x, float y, float z);

	void setUpDirection(glm::vec3 up);

	void setUpDirection(float x, float y, float z);

	void setLocation(float x, float y, float z);

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up{0.0f, 1.0f, 0.0f};

	// TODO Make it possible to change stuff, like fov
	// TODO Should not really use config ...
	float aspectRatio = config::graphics_data::windowWidth / config::graphics_data::windowHeight;
	glm::mat4 projection = glm::perspective(config::graphics_data::fov, aspectRatio, 0.1f, 100.0f);

};

}

#endif /* SRC_GRAPHICS_CAMERA_H_ */
