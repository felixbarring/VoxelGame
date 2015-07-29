
#include "textureDemo.h"

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
#include "../graphics/texture/texture.h"
#include "../graphics/mesh/meshVT.h"

#include "../util/fpsManager.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

TextureDemo::TextureDemo()
{
}

TextureDemo::~TextureDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void TextureDemo::runDemo()
{

	util::FPSManager fpsManager(60);
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

	const char *vertex =
		"#version 330 core \n"
		"in vec3 positionIn; \n"
		"in vec2 texCoordIn; \n"

		"out vec2 texCoord; \n"

		"void main() \n"
		"{ \n"
		"  gl_Position = vec4(positionIn, 1.0f); \n"
		"  texCoord = vec2(texCoordIn.x, 1.0 - texCoordIn.y); \n"
		"} \n";

	const char *frag =
		"#version 330 core \n"
		"in vec2 texCoord; \n"

		"out vec4 color; \n"

		"uniform sampler2D texture1; \n"
		"void main() \n"
		"{ \n"
		"  color = texture(texture1, texCoord); \n"
		"} \n";


	std::map<std::string, int> attributesMap{
		std::pair<std::string, int>("positionIn", 0),
		std::pair<std::string, int>("texCoordIn", 1)
	};

	graphics::ShaderProgram program(vertex, frag, attributesMap);

	std::vector<GLfloat> vertices = {
		0.4f, 0.4f, 0.0f,
		0.4f, -0.4f, 0.0f,
		-0.4f, -0.4f, 0.0f,
		-0.4f, 0.4f, 0.0f
	};

	std::vector<GLfloat> texCoords = {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	std::vector<GLshort> indices = {
		0, 1, 3,
		1, 2, 3
	};

	mesh::MeshVT mesh{vertices, 3, texCoords, 2, indices};
	texture::Texture texture("../resources/0001_grass_side.png");

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		program.bind();

		glActiveTexture(GL_TEXTURE0);
		texture.bind();
		program.setUniformli("texture1", 0);

		mesh.render();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

