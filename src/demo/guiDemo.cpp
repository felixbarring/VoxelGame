
#include "guiDemo.h"

#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "../graphics/spriteBatcher.h"
#include "../graphics/fontMeshBuilder.h"

#include "../util/fpsManager.h"
#include "../util/input.h"

#include "../gui/widget/button.h"
#include "../gui/widget/slider.h"
#include "../gui/widget/label.h"
#include "../gui/widget/textInput.h"
#include "../gui/widget/widgetGroup.h"
#include "../gui/widget/iWidget.h"
#include "../gui/guiUtil.h"

#include "../config/data.h"

namespace demo {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

enum class MenuState {
	MainMenu,
	Play,
	Settings
};

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

	GLFWwindow *window = glfwCreateWindow(WIDTH+400, HEIGHT, "GUI Demo", nullptr, nullptr);
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

	MenuState state = MenuState::MainMenu;

	bool quit = false;
	std::function<void(int)> observer = [&](int id)
	{
		switch(id) {
			case 0: {
				state = MenuState::Play;
				break;
			}
			case 1: {
				state = MenuState::Settings;
				break;
			}
			case 2: {
				quit = true;
				break;
			}
			case 5: {
				state = MenuState::MainMenu;
				break;
			}
			case 6: {
				state = MenuState::MainMenu;
				break;
			}
		}
		std::cout << "A button with id: " << id << " was pressed\n";
	};

	std::function<void(int, float)> observer2 = [&](int id, float value)
	{
		std::cout << value << "\n";
	};

	// ######################################################################################################

	std::shared_ptr<widget::IWidget> button1(new widget::Button{0, 325, 350, 150, 30, observer, "Play"});
	std::shared_ptr<widget::IWidget> button2(new widget::Button{1, 325, 310, 150, 30, observer, "Settings"});
	std::shared_ptr<widget::IWidget> button3(new widget::Button{2, 325, 270, 150, 30, observer, "Quit"});

	widget::WidgetGroup mainWidgetGroup{0, 0, 0, 800, 600, observer};

	mainWidgetGroup.addWidget(button1);
	mainWidgetGroup.addWidget(button2);
	mainWidgetGroup.addWidget(button3);

	// ######################################################################################################

	std::shared_ptr<widget::IWidget> label1(new widget::Label{325, 390, 150, 30, " - Play - "});
	std::shared_ptr<widget::IWidget> button4(new widget::Button{3, 325, 350, 150, 30, observer, "New World"});
	std::shared_ptr<widget::IWidget> button5(new widget::Button{4, 325, 310, 150, 30, observer, "Load World"});
	std::shared_ptr<widget::IWidget> button6(new widget::Button{5, 325, 270, 150, 30, observer, "Back"});

	widget::WidgetGroup playWidgetGroup{0, 0, 0, 800, 600, observer};

	playWidgetGroup.addWidget(label1);
	playWidgetGroup.addWidget(button4);
	playWidgetGroup.addWidget(button5);
	playWidgetGroup.addWidget(button6);

	// ######################################################################################################

	std::shared_ptr<widget::IWidget> label2(new widget::Label{325, 390, 150, 30, " - Settings - "});
	std::shared_ptr<widget::IWidget> textInput(new widget::TextInput{666, 325, 350, 150, 30});
	std::shared_ptr<widget::IWidget> slider(new widget::Slider{666, 325, 310, 150, 30, observer2});
	std::shared_ptr<widget::IWidget> button7(new widget::Button{6, 325, 270, 150, 30, observer, "Back"});

	widget::WidgetGroup settingsWidgetGroup{0, 0, 0, 800, 600, observer};

	settingsWidgetGroup.addWidget(label2);
	settingsWidgetGroup.addWidget(slider);
	settingsWidgetGroup.addWidget(textInput);
	settingsWidgetGroup.addWidget(button7);

	// ######################################################################################################

	widget::WidgetGroup *activeGroup = &mainWidgetGroup;


	util::Input::createInstance(window, WIDTH / 2.0, HEIGHT / 2.0);
	std::shared_ptr<util::Input> input = util::Input::getInstance();
	input->unlockMouse();

	glm::mat4 matrix = gui::createVirtualToScreen(800, 600, 1200, 600);
	glm:: mat4 matrix2 = glm::ortho(0.0f, 1200.0f, 0.0f, 600.0f, -1.0f, 1.0f) * matrix;
	graphics::SpriteBatcher::getInstance().setProjection(matrix2);

	while (!quit && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

		fpsManager.frameStart();
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		input->updateValues();

		double y = input->mouseYPosition - HEIGHT;
		if (y < 0) {
			y = -y;
		} else {
			y = -1;
		}

		glm::vec2 mouse = gui::adjustMouse(800, 600, 1200, 600, input->mouseXPosition, y);


		switch (state) {
			case MenuState::MainMenu: {
				activeGroup = &mainWidgetGroup;
				break;
			}
			case MenuState::Play: {
				activeGroup = &playWidgetGroup;
				break;
			}
			case MenuState::Settings: {
				activeGroup = &settingsWidgetGroup;
				break;
			}
		}

		activeGroup->mouseMoved(mouse.x, mouse.y);
		activeGroup->update();

		if (input->action1Pressed)
			activeGroup->mouseClicked(0, mouse.x, mouse.y);

		if (input->keyWasTyped)
			activeGroup->keyTyped(input->keyTyped);

		activeGroup->draw();

		graphics::SpriteBatcher::getInstance().draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

}

} /* namespace graphics */
