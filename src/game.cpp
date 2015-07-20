
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <GL/glew.h>
#include <glfw3.h>
#include "util/fpsManager.h"

Game::Game()
{
}

Game::~Game()
{
}

GLFWwindow *window;

void Game::run()
{

	FPSManager fpsManager(60);
	const int WIDTH = 800;
	const int HEIGHT = 600;

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Game", nullptr, nullptr);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

		fpsManager.frameStart();

		// Do shit here

		fpsManager.sync();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}




