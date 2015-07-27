
#ifndef SRC_UTIL_INPUT_H_
#define SRC_UTIL_INPUT_H_

namespace input
{

	static bool moveForward;
	static bool moveLeft;
	static bool moveRight;
	static bool moveBackward;

	static bool jump;

	// Mouse ########################################################

	static bool action1;
	static bool action2;

	static void setAllTo(bool value)
	{
		bool moveForward = value;
		bool moveLeft = value;
		bool moveRight = value;
		bool moveBackward = value;
		bool jump = value;
		bool action1 = value;
		bool action2 = value;
	}

}

#endif /* SRC_UTIL_INPUT_H_ */
