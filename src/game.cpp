
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

Game::Game()
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

	FPSManager fpsManager(graphics_data::fps);
	const int WIDTH = graphics_data::windowWidth;
	const int HEIGHT = graphics_data::windowHeight;

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game", nullptr, nullptr);
	if (window == NULL) {
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

	float screenCenterX = WIDTH / 2.0;
	float screenCenterY = HEIGHT / 2.0;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

		fpsManager.frameStart();

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, screenCenterX, screenCenterY);

		glfwPollEvents();

		input::setAllTo(false);

		// Keyboard
		input::moveForward = glfwGetKey(window, input_data::moveForwardButton) == GLFW_PRESS;
		input::moveLeft = glfwGetKey(window, input_data::moveLeftButton) == GLFW_PRESS;
		input::moveRight = glfwGetKey(window, input_data::moveRightButton) == GLFW_PRESS;
		input::moveBackward = glfwGetKey(window, input_data::moveBackwardButton) == GLFW_PRESS;
		input::jump = glfwGetKey(window, input_data::jumpButton) == GLFW_PRESS;

		// Mouse
		input::action1 = glfwGetMouseButton(window, input_data::action1Button) == GLFW_PRESS;
		input::action2 = glfwGetMouseButton(window, input_data::action1Button) == GLFW_PRESS;



		// Do shit here!


		fpsManager.sync();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}




