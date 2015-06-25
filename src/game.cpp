
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <GL/glew.h>
#include <glfw3.h>
#include "graphics/renderer.h"
#include "util/fpsManager.h"

Game::Game() {
	// TODO Auto-generated constructor stub

}

Game::~Game() {
	// TODO Auto-generated destructor stub
}

GLFWwindow *window;

void Game::run()
{

	FPSManager fpsManager(60);

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(800, 600, "Game", nullptr, nullptr);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	const double fps = 60;
	const double timePerFram = 1.0 / fps;
	const double mili = 0.001;
	std::chrono::milliseconds oneMilliSecond(1);

	Renderer renderer;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

		fpsManager.frameStart();

		renderer.render();

		fpsManager.frameStart();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

}




