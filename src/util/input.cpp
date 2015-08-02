#include "input.h"

#include "../config/data.h"

namespace util {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Input::Input(GLFWwindow* w, float centerX, float centerY):
	window{w},
	screenCenterX{centerX},
	screenCenterY{centerY}
{

}

Input::~Input()
{

}

// ########################################################
// Member Functions########################################
// ########################################################

void Input::updateValues()
{
	// Clear everything
	moveForward = false;
	moveLeft = false;
	moveRight = false;
	moveBackward = false;
	jump = false;
	action1 = false;
	action2 = false;

	mouseXMovement = 0.0;
	mouseYMovement = 0.0;

	// Keyboard
	moveForward = glfwGetKey(window, config::input_data::moveForwardButton) == GLFW_PRESS;
	moveLeft = glfwGetKey(window, config::input_data::moveLeftButton) == GLFW_PRESS;
	moveRight = glfwGetKey(window, config::input_data::moveRightButton) == GLFW_PRESS;
	moveBackward = glfwGetKey(window, config::input_data::moveBackwardButton) == GLFW_PRESS;
	jump = glfwGetKey(window, config::input_data::jumpButton) == GLFW_PRESS;

	// Mouse
	action1 = glfwGetMouseButton(window, config::input_data::action1Button) == GLFW_PRESS;
	action2 = glfwGetMouseButton(window, config::input_data::action1Button) == GLFW_PRESS;

	double xOffset, yOffset;
	glfwGetCursorPos(window, &xOffset, &yOffset);
	glfwSetCursorPos(window, screenCenterX, screenCenterY);

	mouseXMovement = screenCenterX - xOffset;
	mouseYMovement = screenCenterY - yOffset;
}

} /* namespace util */
