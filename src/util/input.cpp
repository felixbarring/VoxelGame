#include "input.h"

#include <iostream>

#include "../config/data.h"

namespace util {


static double mouseXOffset;
static double mouseYOffset;

static bool wasTyped = false;
static double typed = ' ';

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouseXOffset = xpos;
	mouseYOffset = ypos;
}

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	wasTyped = true;
	typed = codepoint;
	// std::cout << "Text Input: " << static_cast<char>(codepoint) << "\n";
}


// ########################################################
// Constructor/Destructor #################################
// ########################################################

Input::Input(GLFWwindow* w, float centerX, float centerY):
	window{w},
	screenCenterX{centerX},
	screenCenterY{centerY}
{
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetCharCallback(window, character_callback);
}

// ########################################################
// Member Functions########################################
// ########################################################

void Input::updateValues()
{

	mouseXMovement = 0.0;
	mouseYMovement = 0.0;

	// Keyboard
	moveForwardPressed = glfwGetKey(window, config::input_data::moveForwardButton) == GLFW_PRESS && !moveForwardActive;
	moveLeftPressed = glfwGetKey(window, config::input_data::moveLeftButton) == GLFW_PRESS && !moveLeftActive;
	moveRightPressed = glfwGetKey(window, config::input_data::moveRightButton) == GLFW_PRESS && moveRightActive;
	moveBackwardPressed = glfwGetKey(window, config::input_data::moveBackwardButton) == GLFW_PRESS && moveBackwardActive;
	jumpPressed = glfwGetKey(window, config::input_data::jumpButton) == GLFW_PRESS && jumpActive;
	goDownPressed = glfwGetKey(window, config::input_data::goDownButton) == GLFW_PRESS && goDownActive;

	// Mouse
	action1Pressed = glfwGetMouseButton(window, config::input_data::action1Button) == GLFW_PRESS && !action1Active;
	action2Pressed = glfwGetMouseButton(window, config::input_data::action2Button) == GLFW_PRESS && !action2Active;

	// Keyboard
	moveForwardActive = glfwGetKey(window, config::input_data::moveForwardButton) == GLFW_PRESS;
	moveLeftActive = glfwGetKey(window, config::input_data::moveLeftButton) == GLFW_PRESS;
	moveRightActive = glfwGetKey(window, config::input_data::moveRightButton) == GLFW_PRESS;
	moveBackwardActive = glfwGetKey(window, config::input_data::moveBackwardButton) == GLFW_PRESS;
	jumpActive = glfwGetKey(window, config::input_data::jumpButton) == GLFW_PRESS;
	goDownActive = glfwGetKey(window, config::input_data::goDownButton) == GLFW_PRESS;

	// Mouse
	action1Active = glfwGetMouseButton(window, config::input_data::action1Button) == GLFW_PRESS;
	action2Active = glfwGetMouseButton(window, config::input_data::action2Button) == GLFW_PRESS;

	escapeKeyPressed = glfwGetKey(window, GLFW_KEY_ESCAPE);

	if (mouseLocked) {

		mouseXMovement = screenCenterX - mouseXOffset;
		mouseYMovement = screenCenterY - mouseYOffset;

		centerMouse();
	} else {
		glfwGetCursorPos(window, &mouseXPosition, &mouseYPosition);
	}

	keyWasTyped = wasTyped;
	wasTyped = false;
	keyTyped = typed;

}

void Input::centerMouse()
{
	mouseXOffset = screenCenterX;
	mouseYOffset = screenCenterY;
	glfwSetCursorPos(window, screenCenterX, screenCenterY);
}

void Input::lockMouse()
{
	mouseLocked = true;
}

void Input::unlockMouse()
{
	mouseLocked = false;
}

} /* namespace util */
