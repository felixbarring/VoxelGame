
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
			case 10: {
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

	//std::shared_ptr<widget::IWidget> button1(new widget::Button{0, 325, 350, 150, 30, observer, "Play"});
	std::shared_ptr<widget::IWidget> button2(new widget::Button{1, 325, 310, 150, 30, observer, "Settings"});
	std::shared_ptr<widget::IWidget> button3(new widget::Button{2, 325, 270, 150, 30, observer, "Quit"});

	widget::WidgetGroup widgetGroup1{0, 0, 0, 800, 600, observer};

	//widgetGroup1.addWidget(button1);
	widgetGroup1.addWidget(button2);
	widgetGroup1.addWidget(button3);

	std::shared_ptr<widget::IWidget> slider(new widget::Slider{666, 325, 350, 150, 30, observer2});

	widgetGroup1.addWidget(slider);

	std::shared_ptr<widget::IWidget> textInput(new widget::TextInput{666, 325, 390, 150, 30});

	widgetGroup1.addWidget(textInput);



	std::shared_ptr<widget::IWidget> label1(new widget::Label{325, 390, 150, 30, " - Play - "});
	std::shared_ptr<widget::IWidget> button4(new widget::Button{3, 325, 350, 150, 30, observer, "New World"});
	std::shared_ptr<widget::IWidget> button5(new widget::Button{4, 325, 310, 150, 30, observer, "Load World"});
	std::shared_ptr<widget::IWidget> button6(new widget::Button{5, 325, 270, 150, 30, observer, "Back"});

	widget::WidgetGroup widgetGroup2{0, 0, 0, 800, 600, observer};

	widgetGroup2.addWidget(label1);
	widgetGroup2.addWidget(button4);
	widgetGroup2.addWidget(button5);
	widgetGroup2.addWidget(button6);



	std::shared_ptr<widget::IWidget> label2(new widget::Label{325, 390, 150, 30, " - Settings - "});
	std::shared_ptr<widget::IWidget> button7(new widget::Button{6, 325, 350, 150, 30, observer, "Input"});
	std::shared_ptr<widget::IWidget> button8(new widget::Button{7, 325, 310, 150, 30, observer, "Graphics"});
	std::shared_ptr<widget::IWidget> button9(new widget::Button{8, 325, 270, 150, 30, observer, "Audio"});
	std::shared_ptr<widget::IWidget> button10(new widget::Button{9, 325, 230, 150, 30, observer, "Game"});
	std::shared_ptr<widget::IWidget> button11(new widget::Button{10, 325, 190, 150, 30, observer, "Back"});

	widget::WidgetGroup widgetGroup3{0, 0, 0, 800, 600, observer};

	widgetGroup3.addWidget(label2);
	widgetGroup3.addWidget(button7);
	widgetGroup3.addWidget(button8);
	widgetGroup3.addWidget(button9);
	widgetGroup3.addWidget(button10);
	widgetGroup3.addWidget(button11);


	util::Input input(window, WIDTH / 2.0, HEIGHT / 2.0);
	input.unlockMouse();

	glm::mat4 matrix = gui::createVirtualToScreen(800, 600, 1200, 600);
	glm:: mat4 matrix2 = glm::ortho(0.0f, 1200.0f, 0.0f, 600.0f, -1.0f, 1.0f) * matrix;

	graphics::SpriteBatcher::getInstance().setProjection(matrix2);
	// This resource handler ?!?
	//graphics::FontMeshBuilder fontBuilder{config::font_data::fontLayout, 1024, 1024};

	while (!quit && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

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

		switch(state) {

			case MenuState::MainMenu: {
				widgetGroup1.mouseMoved(mouse.x, mouse.y);
				if (input.action1Pressed) {
					widgetGroup1.mouseClicked(0, mouse.x, mouse.y);
				}

				if (input.keyWasTyped) {
					widgetGroup1.keyTyped(input.keyTyped);
				}

				widgetGroup1.draw();
				break;
			}
			case MenuState::Play: {
				widgetGroup2.mouseMoved(mouse.x, mouse.y);
				if (input.action1Pressed) {
					widgetGroup2.mouseClicked(0, mouse.x, mouse.y);
				}
				widgetGroup2.draw();
				break;
			}
			case MenuState::Settings: {
				widgetGroup3.mouseMoved(mouse.x, mouse.y);
				if (input.action1Pressed) {
					widgetGroup3.mouseClicked(0, mouse.x, mouse.y);
				}
				widgetGroup3.draw();
				break;
			}

		}

		graphics::SpriteBatcher::getInstance().draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

}

} /* namespace graphics */
