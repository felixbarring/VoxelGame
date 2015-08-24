
#include "arrayTextureDemo.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

#include <map>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "../graphics/shaderProgram.h"
#include "../graphics/mesh/meshElement.h"
#include "../config/data.h"
#include "../graphics/texture/textureArray.h"

#include "../util/fpsManager.h"

namespace demo
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ArrayTextureDemo::ArrayTextureDemo()
{
}

ArrayTextureDemo::~ArrayTextureDemo()
{
}

// ########################################################
// Constructor/Destructor #################################
// ########################################################

void ArrayTextureDemo::runDemo()
{

	util::FPSManager fpsManager(60);
	const GLuint WIDTH = 800, HEIGHT = 600;

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW\n";
	}
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Array Texture Demo", nullptr, nullptr);
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

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	const char *vertex =
		"#version 330 core \n"
		"in vec3 positionIn; \n"
		"in vec3 texCoordIn; \n"

		"out vec3 texCoord; \n"

		"void main() \n"
		"{ \n"
		"  gl_Position = vec4(positionIn, 1.0f); \n"
		"  texCoord = vec3(texCoordIn.x, 1.0 - texCoordIn.y, texCoordIn.z); \n"
		"} \n";

	const char *frag =
		"#version 330 core \n"
		"in vec3 texCoord; \n"

		"out vec4 color; \n"

		"uniform sampler2DArray texture1; \n"
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

		-0.3f, 0.0f, 0.0f,
		-0.1f, 0.0f, 0.0f,
		-0.1f, 0.2f, 0.0f,
		-0.3f, 0.2f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.2f, 0.0f, 0.0f,
		0.2f, 0.2f, 0.0f,
		0.0f, 0.2f, 0.0f,

		0.3f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.2f, 0.0f,
		0.3f, 0.2f, 0.0f

	};

	std::vector<GLfloat> texCoords = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 2.0f,
		1.0f, 0.0f, 2.0f,
		1.0f, 1.0f, 2.0f,
		0.0f, 1.0f, 2.0f,

	};

	std::vector<GLshort> indices = {
		0, 1, 2,
		0, 2, 3,

		0+4, 1+4, 2+4,
		0+4, 2+4, 3+4,

		0+8, 1+8, 2+8,
		0+8, 2+8, 3+8
	};

	mesh::MeshElement mesh{vertices, 3, texCoords, 3, indices};

	texture::TextureArray texture{config::cube_data::textures, config::cube_data::TEXTURE_WIDTH, config::cube_data::TEXTURE_HEIGHT};

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

}
