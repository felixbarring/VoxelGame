
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
#include "../graphics/viewDirection.h"
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

	graphics::Transform transform1{0, 0, -5.0f};
	graphics::Transform transform2{2, 0, -5.0f};
	graphics::Transform transform3{4, 0, -5.0f};
	graphics::Transform transform4{6, 0, -5.0f};

	float aspectRatio = 800 / 600;
	glm::mat4 Projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

	glm::mat4 camera = glm::lookAt(
		glm::vec3(0, 0.1, 2), // Camera location
		glm::vec3(0, 0, 0),   // Look at
		glm::vec3(0, 0, 1)    // Head is up
	);

	graphics::Camera::getInstance().setViewMatrix(camera);


	float xAmount = 0;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		transform1.rotateX(0.1);
		transform2.rotateY(0.1);
		transform3.rotateZ(0.1);
		transform4.rotateX(0.1);
		transform4.rotateY(0.1);
		transform4.rotateZ(0.1);

		graphics::CubeBatcher::getInstance().addBatch(1, transform1);
		graphics::CubeBatcher::getInstance().addBatch(2, transform2);
		graphics::CubeBatcher::getInstance().addBatch(3, transform3);
		graphics::CubeBatcher::getInstance().addBatch(4, transform4);

		graphics::CubeBatcher::getInstance().draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

}

