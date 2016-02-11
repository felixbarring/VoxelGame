#include "input.h"

#include <iostream>

#include "../config/data.h"

using namespace config::input_data;

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

void Input::createInstance(GLFWwindow* w, float centerX, float centerY)
{
	input.reset(new Input{w, centerX, centerY});
}

std::shared_ptr<Input> Input::getInstance()
{
	return input;
}


void Input::updateValues()
{

	mouseXMovement = 0.0;
	mouseYMovement = 0.0;

	// Keyboard
	moveForwardPressed = glfwGetKey(window, moveForwardButton) == GLFW_PRESS && !moveForwardActive;
	moveLeftPressed = glfwGetKey(window, moveLeftButton) == GLFW_PRESS && !moveLeftActive;
	moveRightPressed = glfwGetKey(window, moveRightButton) == GLFW_PRESS && !moveRightActive;
	moveBackwardPressed = glfwGetKey(window, moveBackwardButton) == GLFW_PRESS && !moveBackwardActive;
	jumpPressed = glfwGetKey(window, jumpButton) == GLFW_PRESS && !jumpActive;
	goDownPressed = glfwGetKey(window, goDownButton) == GLFW_PRESS && !goDownActive;

	eraseTextPressed = glfwGetKey(window, eraseTextButton) == GLFW_PRESS && !eraseTextActive;
	switchCubePressed = glfwGetKey(window, switchBuildingCube) == GLFW_PRESS && !switchCubeActive;

	// Mouse
	action1Pressed = glfwGetMouseButton(window, action1Button) == GLFW_PRESS && !action1Active;
	action2Pressed = glfwGetMouseButton(window, action2Button) == GLFW_PRESS && !action2Active;

	// Keyboard
	moveForwardActive = glfwGetKey(window, moveForwardButton) == GLFW_PRESS;
	moveLeftActive = glfwGetKey(window, moveLeftButton) == GLFW_PRESS;
	moveRightActive = glfwGetKey(window, moveRightButton) == GLFW_PRESS;
	moveBackwardActive = glfwGetKey(window, moveBackwardButton) == GLFW_PRESS;
	jumpActive = glfwGetKey(window, jumpButton) == GLFW_PRESS;
	goDownActive = glfwGetKey(window, goDownButton) == GLFW_PRESS;

	eraseTextActive = glfwGetKey(window, eraseTextButton) == GLFW_PRESS;
	switchCubeActive = glfwGetKey(window, switchBuildingCube) == GLFW_PRESS;

	// Mouse
	action1Active = glfwGetMouseButton(window, action1Button) == GLFW_PRESS;
	action2Active = glfwGetMouseButton(window, action2Button) == GLFW_PRESS;

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
