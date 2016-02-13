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

#include "model/world/chunk/chunkManager.h"

#include "gui/guiUtil.h"

using namespace std;

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

void Game::run() {

	util::FPSManager fpsManager(config::graphics_data::fps);

	if (!glfwInit()) {
		cout << "Failed to initialize GLFW\n";
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
	int refreshRate = viewMode->refreshRate;

	cout << "Width: " << width << "\n";
	cout << "Height: " << height << "\n";
	cout << "Refresh Rate: " << refreshRate << "\n";

	//int WIDTH = width;
	//int HEIGHT = height;

	int WIDTH = 800;
	int HEIGHT = 600;

	config::graphics_data::windowWidth = WIDTH;
	config::graphics_data::windowHeight = HEIGHT;

	//GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Voxel Game", glfwGetPrimaryMonitor(), nullptr);
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Voxel Game", nullptr,
			nullptr);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (window == nullptr) {
		cout << "Failed to open GLFW window.\n";
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(-1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		cout << "Failed to initialize GLEW\n";

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.47f, 0.76f, 0.93f, 1.0f);

	config::graphics_data::windowWidth = WIDTH;
	config::graphics_data::windowHeight = HEIGHT;

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	util::Input::createInstance(window, WIDTH / 2.0, HEIGHT / 2.0);

	m_mainMenu.reset(new MainMenu(this));
	m_currentState = m_mainMenu;

	while (!m_quit && glfwWindowShouldClose(window) == 0) {
		fpsManager.frameStart();
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_currentState->update(fpsManager.frameTime());

		fpsManager.sync();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

}

void Game::createNewWorld(string name) {
	m_inGame.reset(new InGame(this, name));
	chunk::ChunkManager::getInstance().createNewWorld();
	m_currentState = m_inGame;
}

void Game::loadExistingWorld(string name) {
	m_inGame.reset(new InGame(this, name));
	chunk::ChunkManager::getInstance().loadWorld(name);
	m_currentState = m_inGame;
}

void Game::changeStateToMainMenu() {
	m_currentState = m_mainMenu;
}

void Game::quitGame() {
	m_quit = true;
}

