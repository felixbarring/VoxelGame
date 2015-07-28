
#ifndef SRC_UTIL_INPUT_H_
#define SRC_UTIL_INPUT_H_

namespace util
{

class Input {

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

	static bool moveForward;
	static bool moveLeft;
	static bool moveRight;
	static bool moveBackward;

	static bool jump;

	// Mouse

	static bool action1;
	static bool action2;

	static float mouseXMovement;
	static float mouseYMovement;

private:

	static GLFWwindow* window;

	float screenCenterX;
	float screenCenterY;

};

}

#endif /* SRC_UTIL_INPUT_H_ */
