
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

	virtual ~Camera() {};

// ########################################################
// Member Functions########################################
// ########################################################

public:

	static Camera& getInstance()
	{
		static Camera INSTANCE{0,0,0};
		return INSTANCE;
	}

	void setViewMatrix(glm::mat4 view);

	glm::mat4 getViewMatrix();

	void setProjectionMatrix();

	glm::mat4& getProjectionMatrix();

	glm::vec3& getPosition();

	void updateView(glm::vec3 position, glm::vec3 direction, glm::vec3 up);


// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up{0.0f, 1.0f, 0.0f};

	// TODO Update this in an intelligent way
	float aspectRatio = config::graphics_data::windowWidth / static_cast<float>(config::graphics_data::windowHeight);
	glm::mat4 projection = glm::perspective(config::graphics_data::fov, aspectRatio, 0.1f, 1000.0f);
	glm::mat4 view;

};

}

#endif /* SRC_GRAPHICS_CAMERA_H_ */
