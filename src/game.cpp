
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util/fpsManager.h"
#include "config/data.h"

#include "util/input.h"


// ########################################################
// Constructor/Destructor #################################
// ########################################################

Game::Game():
	gameState{GameState::InGame}
{

}

Game::~Game()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void Game::run()
{

	util::FPSManager fpsManager(config::graphics_data::fps);
	const int WIDTH = config::graphics_data::windowWidth;
	const int HEIGHT = config::graphics_data::windowHeight;

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	util::Input input(window, WIDTH / 2.0, WIDTH / 2.0);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

		fpsManager.frameStart();
		input.updateValues();

		// Do shit here!

		fpsManager.sync();

		glfwSwapBuffers(window);
		//glfwPollEvents();
	}
	glfwTerminate();
}

