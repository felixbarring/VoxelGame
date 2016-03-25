#ifndef SRC_UTIL_INPUT_H_
#define SRC_UTIL_INPUT_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include <SFML/Window.hpp>

namespace util {

class Input {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Input(GLFWwindow* w, float centerX, float centerY);

public:

	virtual ~Input() { };

	static void createInstance(GLFWwindow* w, float centerX, float centerY);

	static std::shared_ptr<Input> getInstance();

// ########################################################
// Member Functions########################################
// ########################################################

	void updateValues();

	void centerMouse();

	void lockMouse();

	void unlockMouse();

	void setWindow(sf::Window *window);

// ########################################################
// Implementation #########################################
// ########################################################

	// Keyboard
	bool moveForwardPressed {false};
	bool moveForwardActive {false};

	bool moveLeftPressed {false};
	bool moveLeftActive {false};

	bool moveRightPressed {false};
	bool moveRightActive {false};

	bool moveBackwardPressed {false};
	bool moveBackwardActive {false};

	bool jumpPressed {false};
	bool jumpActive {false};

	bool goDownPressed {false};
	bool goDownActive {false};

	// Mouse
	bool action1Pressed {false};
	bool action1Active {false};

	bool action2Pressed {false};
	bool action2Active {false};

	float mouseXMovement {0.0f};
	float mouseYMovement {0.0f};

	double mouseXPosition {0.0};
	double mouseYPosition {0.0};

	double mouseVirtualAdjustedX {0.0};
	double mouseVirtualAdjustedY {0.0};

	bool escapeKeyPressed {false};

	bool openTerminalPressed {false};

	bool eraseTextPressed {false};
	bool eraseTextActive {false};

	bool switchCubeActive {false};
	bool switchCubePressed {false};

	bool keyWasTyped {false};
	char keyTyped {' '};

private:

	GLFWwindow* window;

	float screenCenterX;
	float screenCenterY;

	bool mouseLocked {true};

	sf::Window *m_window;
};

static std::shared_ptr<Input> input;

} /* namespace util */

#endif /* SRC_UTIL_INPUT_H_ */
