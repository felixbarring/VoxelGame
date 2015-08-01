
#ifndef SRC_UTIL_INPUT_H_
#define SRC_UTIL_INPUT_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace util {

class Input {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	Input(GLFWwindow* w, float centerX, float centerY);
	virtual ~Input();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	void updateValues();

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	// Keyboard

	bool moveForward{false};
	bool moveLeft{false};
	bool moveRight{false};
	bool moveBackward{false};

	bool jump{false};

	// Mouse

	bool action1{false};
	bool action2{false};

	float mouseXMovement{0.0f};
	float mouseYMovement{0.0f};

private:

	GLFWwindow* window;

	float screenCenterX;
	float screenCenterY;

};

} /* namespace util */

#endif /* SRC_UTIL_INPUT_H_ */
