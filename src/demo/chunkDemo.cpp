
#include "chunkDemo.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../graphics/chunkBatcher.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/graphicalChunk.h"
#include "../graphics/camera.h"
#include "../graphics/viewDirection.h"
#include "../graphics/texture/textureArray.h"
#include "../util/fpsManager.h"
#include "../util/voxel.h"
#include "../config/data.h"

#include "../model/world/chunk/chunk.h"

namespace demo
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

void ChunkDemo::runDemo()
{
	util::FPSManager fpsManager(100);
	const GLuint WIDTH = 800, HEIGHT = 600;

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW\n";
	}
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Chunk Demo", nullptr, nullptr);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	graphics::Camera& camera = graphics::Camera::getInstance();

	float screenCenterX = WIDTH / 2.0;
	float screenCenterY = HEIGHT / 2.0;

	chunk::Chunk chunk{0,0,0};

	chunk.setCube(0,0,0, config::cube_data::AIR);
	chunk.setCube(8,8,8, config::cube_data::AIR);

	graphics::ViewDirection viewDirection;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, screenCenterX, screenCenterY);

		viewDirection.changeViewDirection(screenCenterX - xpos, screenCenterY - ypos);

		camera.updateView(glm::vec3(8,17,-5), viewDirection.getViewDirection(), viewDirection.getUpDirection());

		glm::mat4 mat;

		graphics::ChunkBatcher::getInstance().draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

}

