
#include "playerCollisionDemo.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../graphics/chunkBatcher.h"
#include "../graphics/cubeBatcher.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/graphicalChunk.h"
#include "../graphics/camera.h"
#include "../graphics/texture/textureArray.h"
#include "../util/fpsManager.h"
#include "../util/voxel.h"
#include "../config/data.h"

#include "../model/world/chunk/chunk.h"
#include "../model/world/entity/player.h"
#include "../util/input.h"
#include "../util/checkSystem.h"

namespace demo {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

PlayerCollisionDemo::PlayerCollisionDemo()
{
}

PlayerCollisionDemo::~PlayerCollisionDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void PlayerCollisionDemo::runDemo()
{

	// TODO Remove from here!?!

	util::check_system::checkStuff();

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

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Player Collision Demo", nullptr, nullptr);
	if (window == nullptr) {
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

	float screenCenterX = WIDTH / 2.0;
	float screenCenterY = HEIGHT / 2.0;

	util::Input input{window, screenCenterX, screenCenterY};

	entity::Player player{input};
	player.setLocation(7, 7, 7);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();

		input.updateValues();
		player.update(0.0f);

		graphics::ChunkBatcher::getInstance().draw();
		graphics::CubeBatcher::getInstance().draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

} /* namespace demo */
