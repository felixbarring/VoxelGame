
#include "input.h"

#include <SFML/Window.hpp>
#include <iostream>

#include "../config/data.h"

using namespace config::input_data;

namespace util {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Input::Input(GLFWwindow* w, float centerX, float centerY)
	: screenCenterX(centerX),
	  screenCenterY(centerY)
		{
}

// ########################################################
// Member Functions########################################
// ########################################################

void Input::createInstance(GLFWwindow* w, float centerX, float centerY) {
	input.reset(new Input {w, centerX, centerY});
}

std::shared_ptr<Input> Input::getInstance() {
	return input;
}

void Input::updateValues() {

	mouseXMovement = 0.0;
	mouseYMovement = 0.0;

	// Keyboard
	moveForwardPressed = sf::Keyboard::isKeyPressed(moveForwardButton)
		&& !moveForwardActive;
	moveLeftPressed = sf::Keyboard::isKeyPressed(moveLeftButton)
		&& !moveLeftActive;
	moveRightPressed = sf::Keyboard::isKeyPressed(moveRightButton)
		&& !moveRightActive;
	moveBackwardPressed = sf::Keyboard::isKeyPressed(moveBackwardButton)
		&& !moveBackwardActive;
	jumpPressed = sf::Keyboard::isKeyPressed(jumpButton) && !jumpActive;
	goDownPressed = sf::Keyboard::isKeyPressed(goDownButton) && !goDownActive;

	eraseTextPressed = sf::Keyboard::isKeyPressed(eraseTextButton)
		&& !eraseTextActive;
	switchCubePressed = sf::Keyboard::isKeyPressed(switchBuildingCube)
		&& !switchCubeActive;

	// Mouse
	action1Pressed = sf::Mouse::isButtonPressed(action1Button)
		&& !action1Active;
	action2Pressed = sf::Mouse::isButtonPressed(action2Button)
		&& !action2Active;

	// Keyboard
	moveForwardActive = sf::Keyboard::isKeyPressed(moveForwardButton);
	moveLeftActive = sf::Keyboard::isKeyPressed(moveLeftButton);
	moveRightActive = sf::Keyboard::isKeyPressed(moveRightButton);
	moveBackwardActive = sf::Keyboard::isKeyPressed(moveBackwardButton);
	jumpActive = sf::Keyboard::isKeyPressed(jumpButton);
	goDownActive = sf::Keyboard::isKeyPressed(goDownButton);

	eraseTextActive = sf::Keyboard::isKeyPressed(eraseTextButton);
	switchCubeActive = sf::Keyboard::isKeyPressed(switchBuildingCube);

	openTerminalPressed = sf::Keyboard::isKeyPressed(openTerminal);

	// Mouse
	action1Active = sf::Mouse::isButtonPressed(action1Button);
	action2Active = sf::Mouse::isButtonPressed(action2Button);

	escapeKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

	if (mouseLocked) {
		auto ve = sf::Mouse::getPosition(*m_window);
		double mouseXOffset = ve.x;
		double mouseYOffset = ve.y;

		mouseXMovement = screenCenterX - mouseXOffset;
		mouseYMovement = screenCenterY - mouseYOffset;
		centerMouse();
	}
	else {
		auto vec = sf::Mouse::getPosition(*m_window);
		mouseXPosition = vec.x;
		mouseYPosition = vec.y;
	}

	keyWasTyped = false;

	// Loop thru all the key events and use it as the key typed
    sf::Event event;
	while (m_window->pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			m_window->close();
		else if (event.type == sf::Event::TextEntered) {
			char c = static_cast<char>(event.text.unicode);
			if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
					('a' <= c && c <= 'z') || ('0' <= c && c <= '9') ||
					c == ' ') {
				keyWasTyped = true;
				keyTyped = static_cast<char>(event.text.unicode);
			}
		}
	}
}

void Input::centerMouse() {
	sf::Vector2<int> vec(screenCenterX, screenCenterY);
	sf::Mouse::setPosition(vec, *m_window);
}

void Input::lockMouse() {
	mouseLocked = true;
}

void Input::unlockMouse() {
	mouseLocked = false;
}

void Input::setWindow(sf::Window *window) {
	m_window = window;
}

} /* namespace util */
