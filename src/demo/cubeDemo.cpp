
#include "cubeDemo.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shaderProgram.h"
#include "../graphics/texture/textureArray.h"
#include "../graphics/transform.h"
#include "../graphics/texturedCube.h"
#include "../graphics/cubeBatcher.h"
#include "../graphics/camera.h"
#include "../util/fpsManager.h"
#include "../config/data.h"

namespace demo
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

CubeDemo::CubeDemo()
{
}

CubeDemo::~CubeDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void CubeDemo::runDemo()
{

	util::FPSManager fpsManager(100);
	const GLuint WIDTH = 800, HEIGHT = 600;

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
	}
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Textured Cubes Demo", nullptr, nullptr);
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

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	std::shared_ptr<graphics::TexturedCube> batch1(new graphics::TexturedCube{2, 0, -1.0f, 0});
	std::shared_ptr<graphics::TexturedCube> batch2{new graphics::TexturedCube{-2, 0, -1.0f, 1}};
	std::shared_ptr<graphics::TexturedCube> batch3{new graphics::TexturedCube{0, 0, -1.0f, 2}};

	graphics::CubeBatcher::getInstance().addBatch(batch1);
	graphics::CubeBatcher::getInstance().addBatch(batch2);
	graphics::CubeBatcher::getInstance().addBatch(batch3);

	graphics::Camera::getInstance().setLocation(0, 0.2, 2);
	graphics::Camera::getInstance().setViewDirection(0, -1, 0);
	graphics::Camera::getInstance().setUpDirection(0, 0, 1.0);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		batch1.get()->getTransform().rotateX(0.05);
		batch2.get()->getTransform().rotateY(0.05);
		batch3.get()->getTransform().rotateZ(0.05);

		graphics::CubeBatcher::getInstance().draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

}

