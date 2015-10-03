
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util/input.h"
#include "util/fpsManager.h"
#include "config/data.h"

#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"

#include "inGame.h"
#include "mainMenu.h"


// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

void Game::run()
{

	util::FPSManager fpsManager(100);
	int WIDTH = 1920, HEIGHT = 1080;

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW\n";
	}
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Voxel Game", glfwGetPrimaryMonitor(), nullptr);


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

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	util::Input input(window, WIDTH / 2.0, HEIGHT / 2.0);

	entity::Player player{input};
	player.setLocation(7, 7, 7);

	InGame inGame{this, input};
	MainMenu mainMenu{this, input};

	while (!quit && glfwWindowShouldClose(window) == 0) {

		fpsManager.frameStart();
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);

		// inGame.update();

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
	quit= true;
}


