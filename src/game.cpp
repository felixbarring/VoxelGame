
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "util/input.h"
#include "util/fpsManager.h"
#include "config/data.h"

#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"

#include "inGame.h"
#include "mainMenu.h"

#include "gui/guiUtil.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

void Game::run()
{

	util::FPSManager fpsManager(config::graphics_data::fps);

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW\n";
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *viewMode = glfwGetVideoMode(primaryMonitor);

	int width = viewMode->width;
	int height = viewMode->height;
	int refreshRate =viewMode->refreshRate;

	std::cout << "Width: " << width << "\n";
	std::cout << "Height: " << height << "\n";
	std::cout << "Refresh Rate: " << refreshRate << "\n";

	//int WIDTH = width;
	//int HEIGHT = height;

	int WIDTH = 800;
	int HEIGHT = 600;

	config::graphics_data::windowWidth = WIDTH;
	config::graphics_data::windowHeight = HEIGHT;

	//GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Voxel Game", glfwGetPrimaryMonitor(), nullptr);
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Voxel Game", nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Failed to open GLFW window.\n";
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(-1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		std::cout << "Failed to initialize GLEW\n";

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.47f, 0.76f, 0.93f, 1.0f);

	config::graphics_data::windowWidth = WIDTH;
	config::graphics_data::windowHeight = HEIGHT;

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	util::Input::createInstance(window, WIDTH / 2.0, HEIGHT / 2.0);

	glm::mat4 matrix = gui::createVirtualToScreen(config::graphics_data::virtualWidth,
			config::graphics_data::virtualHeight,
			static_cast<float>(WIDTH),
			static_cast<float>(HEIGHT));

	glm:: mat4 matrix2 = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT), -1.0f, 1.0f) * matrix;
	graphics::SpriteBatcher::getInstance().setProjection(matrix2);

	InGame inGame{this};
	MainMenu mainMenu{this};

	while (!quit && glfwWindowShouldClose(window) == 0) {

		fpsManager.frameStart();
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);

		if (state == GameState::MainMenu) {
			mainMenu.update();
		} else if (state == GameState::InGame){
			inGame.update();
		}

		fpsManager.sync();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

}

void Game::changeStateToIngame()
{
	state = GameState::InGame;
}

void Game::changeStateToMainMenu()
{
	state = GameState::MainMenu;
}

void Game::quitGame()
{
	quit = true;
}


