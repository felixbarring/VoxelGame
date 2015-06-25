
#include "cubeDemo.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

#include <map>

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

#include "../graphics/shaderProgram.h"
#include "../graphics/texture.h"
#include "../graphics/mesh/meshVT.h"
#include "../graphics/mesh/meshVNT.h"
#include "../graphics/texturedCube.h"
#include "../graphics/transform.h"

#include "../util/fpsManager.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

CubeDemo::CubeDemo() {
	// TODO Auto-generated constructor stub

}

CubeDemo::~CubeDemo() {
	// TODO Auto-generated destructor stub
}

// ########################################################
// Member Functions########################################
// ########################################################

void CubeDemo::runDemo()
{

	FPSManager fpsManager(60);
	const GLuint WIDTH = 800, HEIGHT = 600;

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
	}
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Texture Demo", nullptr, nullptr);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	const char *vertex =
		"#version 330 core \n"

		"in vec3 positionIn; \n"
		"in vec3 normalIn; \n"
		"in vec2 texCoordIn; \n"

		// Matrices
		"uniform mat3 Normal; \n"
		"uniform mat4 ModelViewProjection; \n"

		"out vec3 faceNormal; \n"
		"out vec2 texCoord; \n"

		"void main(){ \n"
		"  faceNormal = normalize( Normal * normalIn); \n"
		"  texCoord = vec2(texCoordIn.x, 1 - texCoordIn.y); \n"
		//"  texCoord = texCoordIn; \n"
		"  gl_Position =  ModelViewProjection * vec4(positionIn, 1); \n"
		"} \n";

	const char *fragment =
		"#version 330 core \n"

		"in vec3 faceNormal; \n"
		"in vec2 texCoord; \n"

		"uniform sampler2D texture1; \n"

		"out vec4 color; \n"

		"void main(){ \n"
		"  color = texture(texture1, texCoord); \n"
		"} \n";


	// Use Smart Pointer
	std::map<std::string, int> *map =
			new std::map<std::string, int> {std::pair<std::string, int>("positionIn", 0),
					std::pair<std::string, int>("normalIn", 1), std::pair<std::string, int>("texCoordIn", 2)};

	ShaderProgram program(vertex, fragment, map);

	TexturedCube texturedCube{0, 0};
	Texture texture("../resources/terrain.png");

	float aspectRatio = 800 / 600;
	glm::mat4 Projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

    glm::mat4 camera = glm::lookAt(
		glm::vec3(0, 0.1, 2), // Camera location
		glm::vec3(0, 0, 0),   // Look at
		glm::vec3(0, 0, 1)    // Head is up
	);

    Transform transform(0.0f, 0.0f, 0.0f);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.bind();

		glActiveTexture(GL_TEXTURE0);
		texture.bind();
		program.setUniformli("texture1", 0);

		transform.rotateX(0.05);

		glm::mat4 ModelView = camera * transform.getMatrix();

		program.setUniformMatrix4f("ModelView", ModelView);

		glm::mat3 Normal = glm::mat3(glm::vec3(ModelView[0]), glm::vec3(ModelView[1]), glm::vec3(ModelView[2]));
		program.setUniformMatrix3f("Normal", Normal);

    	glm::mat4 ModelViewProjection = Projection * ModelView;
        program.setUniformMatrix4f("ModelViewProjection", ModelViewProjection);

		texturedCube.draw();

		texturedCube.getModelMatrix();
		texturedCube.draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}


