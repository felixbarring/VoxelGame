
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

	void lockMouse();

	void unlockMouse();

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	// Keyboard
	bool moveForwardPressed{false};
	bool moveForwardActive{false};

	bool moveLeftPressed{false};
	bool moveLeftActive{false};

	bool moveRightPressed{false};
	bool moveRightActive{false};

	bool moveBackwardPressed{false};
	bool moveBackwardActive{false};

	bool jumpPressed{false};
	bool jumpActive{false};

	bool goDownPressed{false};
	bool goDownActive{false};

	// Mouse
	bool action1Pressed{false};
	bool action1Active{false};

	bool action2Pressed{false};
	bool action2Active{false};

	float mouseXMovement{0.0f};
	float mouseYMovement{0.0f};

	double mouseXPosition = 0.0;
	double mouseYPosition = 0.0;

	bool escapeKeyPressed{false};

private:

	GLFWwindow* window;

	float screenCenterX;
	float screenCenterY;

	bool mouseLocked = true;

};




} /* namespace util */

#endif /* SRC_UTIL_INPUT_H_ */
