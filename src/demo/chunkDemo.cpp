
#include "chunkDemo.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/chunkBatcher.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/graphicalChunk.h"
#include "../graphics/camera.h"
#include "../graphics/texture/textureArray.h"
#include "../util/fpsManager.h"
#include "../config/data.h"
#include "../voxel.h"

#include "../model/world/chunk/chunk.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ChunkDemo::ChunkDemo()
{
}

ChunkDemo::~ChunkDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void ChunkDemo::runDemo()
{
	util::FPSManager fpsManager(100);
	const GLuint WIDTH = 800, HEIGHT = 600;

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Chunk Demo", nullptr, nullptr);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(-1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Camera& camera = Camera::getInstance();

	float screenCenterX = WIDTH / 2.0;
	float screenCenterY = HEIGHT / 2.0;

	Chunk chunk{0,0,0};

	chunk.setCube(0,0,0, config::cube_data::AIR);
	chunk.setCube(8,8,8, config::cube_data::AIR);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera.moveForward(0.1f);
		}

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, screenCenterX, screenCenterY);

		camera.changeViewDirection(screenCenterX - xpos, screenCenterY - ypos);

		ChunkBatcher::getInstance().draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

