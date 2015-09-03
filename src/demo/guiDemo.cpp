
#include "guiDemo.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "../graphics/spriteBatcher.h"
#include "../util/fpsManager.h"
#include "../util/input.h"

#include "../gui/widget/button.h"
#include "../gui/guiUtil.h"

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

	GLFWwindow *window = glfwCreateWindow(WIDTH+400, HEIGHT, "Sprite Demo", nullptr, nullptr);
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

	glViewport(0, 0, WIDTH+400, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	std::function<void(int)> observer = [](int id){ std::cout << "A button with id: " << id << " was pressed\n"; };

	widget::Button button(0, 350, 50, 100, 100, observer);
	widget::Button button2(1, 350, 200, 100, 100, observer);
	widget::Button button3(2, 350, 350, 100, 100, observer);

	util::Input input(window, WIDTH / 2.0, HEIGHT / 2.0);
	input.unlockMouse();

	glm::mat4 matrix = gui::crateVirtualToScreen(800, 600, 1200, 600);

	glm:: mat4 matrix2 = glm::ortho(0.0f, 1200.0f, 0.0f, 600.0f, -1.0f, 1.0f) * matrix;

	graphics::SpriteBatcher::getInstance().setProjection(matrix2);

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

		glm::vec2 mouse = gui::adjustMouse(800, 600, 1200, 600, input.mouseXPosition, y);

		button.mouseMoved(mouse.x, mouse.y);
		button2.mouseMoved(mouse.x, mouse.y);
		button3.mouseMoved(mouse.x, mouse.y);

		if (input.action1Pressed) {
			button.mouseClicked(0, mouse.x, mouse.y);
			button2.mouseClicked(0, mouse.x, mouse.y);
			button3.mouseClicked(0, mouse.x, mouse.y);
		}

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
