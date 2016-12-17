
#include "input.h"

#include <SFML/Window.hpp>
#include <iostream>

#include "../config/data.h"

using namespace config::input_data;
using namespace sf;

namespace util {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Input::Input(float centerX, float centerY)
    : m_screenCenterX(centerX),
      m_screenCenterY(centerY)
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void Input::createInstance(float centerX, float centerY) {
    input.reset(new Input {centerX, centerY});
}

std::shared_ptr<Input> Input::getInstance() {
    return input;
}

void Input::updateValues() {

    mouseXMovement = 0.0;
    mouseYMovement = 0.0;

    // Keyboard
    moveForwardPressed = Keyboard::isKeyPressed(moveForwardButton) && !moveForwardActive;
    moveLeftPressed = Keyboard::isKeyPressed(moveLeftButton) && !moveLeftActive;
    moveRightPressed = Keyboard::isKeyPressed(moveRightButton) && !moveRightActive;
    moveBackwardPressed = Keyboard::isKeyPressed(moveBackwardButton) && !moveBackwardActive;
    jumpPressed = Keyboard::isKeyPressed(jumpButton) && !jumpActive;
    goDownPressed = Keyboard::isKeyPressed(goDownButton) && !goDownActive;

    eraseTextPressed = Keyboard::isKeyPressed(eraseTextButton) && !eraseTextActive;
    switchCubePressed = Keyboard::isKeyPressed(switchBuildingCube) && !switchCubeActive;

    openTerminalPressed = Keyboard::isKeyPressed(openTerminal) && !openTerminalActive;
    tabPressed = Keyboard::isKeyPressed(tab) && !tabActive;
    enterPressed = Keyboard::isKeyPressed(enter) && !enterActive;
    upPressed = Keyboard::isKeyPressed(up) && !upActive;
    downPressed = Keyboard::isKeyPressed(down) && !downActive;

    escapeKeyPressed = Keyboard::isKeyPressed(escape) && !escapeKeyActive;

    // Mouse
    action1Pressed = Mouse::isButtonPressed(action1Button) && !action1Active;
    action2Pressed = Mouse::isButtonPressed(action2Button) && !action2Active;

    // Keyboard
    moveForwardActive = Keyboard::isKeyPressed(moveForwardButton);
    moveLeftActive = Keyboard::isKeyPressed(moveLeftButton);
    moveRightActive = Keyboard::isKeyPressed(moveRightButton);
    moveBackwardActive = Keyboard::isKeyPressed(moveBackwardButton);
    jumpActive = Keyboard::isKeyPressed(jumpButton);
    goDownActive = Keyboard::isKeyPressed(goDownButton);

    eraseTextActive = Keyboard::isKeyPressed(eraseTextButton);
    switchCubeActive = Keyboard::isKeyPressed(switchBuildingCube);

    openTerminalActive = Keyboard::isKeyPressed(openTerminal);
    tabActive = Keyboard::isKeyPressed(tab);
    enterActive = Keyboard::isKeyPressed(enter);
    upActive = Keyboard::isKeyPressed(up);
    downActive = Keyboard::isKeyPressed(down);

    escapeKeyActive = Keyboard::isKeyPressed(escape);

	// Mouse
	action1Active = Mouse::isButtonPressed(action1Button);
	action2Active = Mouse::isButtonPressed(action2Button);

    if (m_mouseLocked) {
        auto ve = Mouse::getPosition(*m_window);
        double mouseXOffset = ve.x;
        double mouseYOffset = ve.y;

        mouseXMovement = m_screenCenterX - mouseXOffset;
        mouseYMovement = m_screenCenterY - mouseYOffset;
        centerMouse();
    }
    else {
        auto vec = Mouse::getPosition(*m_window);
        mouseXPosition = vec.x;
        mouseYPosition = vec.y;
    }

    keyWasTyped = false;

    // Loop thru all the key events and use it as the key typed
    Event event;
    while (m_window->pollEvent(event)) {
        if (event.type == Event::Closed)
            m_window->close();
        else if (event.type == Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c >= 32) {
                keyWasTyped = true;
                keyTyped = static_cast<char>(event.text.unicode);
            }
        }
    }
}

void Input::centerMouse() {
    Vector2<int> vec(m_screenCenterX, m_screenCenterY);
    Mouse::setPosition(vec, *m_window);
}

void Input::lockMouse() {
    m_mouseLocked = true;
}

void Input::unlockMouse() {
    m_mouseLocked = false;
}

void Input::setWindow(Window *window) {
    m_window = window;
}

} /* namespace util */
