
#include "guiDemo.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../graphics/spriteBatcher.h"
#include "../util/fpsManager.h"
#include "../util/input.h"

#include "../gui/widget/button.h"

namespace demo {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

GuiDemo::GuiDemo()
{
}

GuiDemo::~GuiDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void GuiDemo::runDemo()
{
	util::FPSManager fpsManager(60);
	const GLuint WIDTH = 800, HEIGHT = 600;

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW\n";
	}
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Sprite Demo", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to open GLFW window.\n";
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(-1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW\n";
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	widget::Button button(0, 300, 50, 200, 200);
	widget::Button button2(0, 300, 300, 200, 200);
	widget::Button button3(0, 300, 550, 200, 200);

	util::Input input(window, WIDTH / 2.0, HEIGHT / 2.0);
	input.unlockMouse();

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		input.updateValues();

		double y = input.mouseYPosition - HEIGHT;
		if (y < 0) {
			y = -y;
		} else {
			y = -1;
		}
		std::cout << "Mouse : " << input.mouseXPosition << ", " << y << "\n";
		button.mouseMoved(input.mouseXPosition, y);
		button2.mouseMoved(input.mouseXPosition, y);
		button3.mouseMoved(input.mouseXPosition, y);

		button.draw();
		button2.draw();
		button3.draw();
		graphics::SpriteBatcher::getInstance().draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

} /* namespace graphics */
