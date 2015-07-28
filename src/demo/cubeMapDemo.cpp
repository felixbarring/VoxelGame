
#include "cubeMapDemo.h"

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shaderProgram.h"
#include "../graphics/camera.h"
#include "../graphics/texture/textureCubeMap.h"
#include "../graphics/mesh/meshV.h"
#include "../util/fpsManager.h"
#include "../graphics/cubeMap.h"


// ########################################################
// Constructor/Destructor #################################
// ########################################################

CubeMapDemo::CubeMapDemo()
{
}


CubeMapDemo::~CubeMapDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void CubeMapDemo::runDemo()
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

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Cube Map Demo", nullptr, nullptr);
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

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    Camera& camera = Camera::getInstance();

    CubeMap skyBox{
    	"../resources/skybox/right.jpg",
		"../resources/skybox/left.jpg",
		"../resources/skybox/top.jpg",
		"../resources/skybox/bottom.jpg",
		"../resources/skybox/back.jpg",
		"../resources/skybox/front.jpg",
		2048, 2048
    };

    //float aspectRatio = WIDTH / HEIGHT;

	float screenCenterX = WIDTH / 2;
	float screenCenterY = HEIGHT / 2;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

		fpsManager.frameStart();

		glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LEQUAL);

        double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, screenCenterX, screenCenterY);
		camera.changeViewDirection(screenCenterX - xpos, screenCenterY - ypos);

		skyBox.render(camera);

        fpsManager.sync();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}

