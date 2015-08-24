
#include "spriteDemo.h"

#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shaderProgram.h"
#include "../graphics/camera.h"
#include "../graphics/viewDirection.h"
#include "../graphics/texture/textureArray.h"
#include "../graphics/mesh/meshElement.h"
#include "../util/fpsManager.h"
#include "../graphics/cubeMap.h"

namespace demo {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

SpriteDemo::SpriteDemo()
{
}

SpriteDemo::~SpriteDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void SpriteDemo::runDemo()
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

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Sprite Demo", nullptr, nullptr);
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

		"uniform mat4 projection; \n"

		"out vec3 texCoord; \n"

		"void main() \n"
		"{ \n"
		"  gl_Position = projection * vec4(positionIn, 1.0f); \n"
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

		0.0f, 0.0f, 0.0f,
		200.0f, 0.0f, 0.0f,
		200.0f, 200.0f, 0.0f,
		0.0f, 200.0f, 0.0f,

	};

	std::vector<GLfloat> texCoords = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	std::vector<GLshort> indices = {
		0, 1, 2,
		0, 2, 3,
	};

	mesh::MeshElement mesh{vertices, 3, texCoords, 3, indices};

	texture::TextureArray texture{config::cube_data::textures, config::cube_data::TEXTURE_WIDTH, config::cube_data::TEXTURE_HEIGHT};

	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		program.bind();

		glActiveTexture(GL_TEXTURE0);
		texture.bind();
		program.setUniformli("texture1", 0);
		program.setUniformMatrix4f("projection", projection);

		mesh.render();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

} /* namespace graphics */
