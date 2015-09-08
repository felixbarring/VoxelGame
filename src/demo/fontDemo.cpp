
#include "fontDemo.h"

#include <iostream>
#include <map>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../util/fpsManager.h"

#include "../config/data.h"

#include "../graphics/texture/texture.h"
#include "../graphics/fontMeshBuilder.h"
#include "../graphics/mesh/meshElement.h"
#include "../graphics/sprite.h"
#include "../graphics/shaderProgram.h"


namespace demo {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

FontDemo::FontDemo()
{
}

FontDemo::~FontDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################


void FontDemo::runDemo()
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

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Font Demo", nullptr, nullptr);
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
		"in vec2 texCoordIn; \n"

		"uniform mat4 projection; \n"

		"out vec2 texCoord; \n"

		"void main() \n"
		"{ \n"
		"  gl_Position = projection * vec4(positionIn, 1.0f); \n"
		"  texCoord = texCoordIn; \n"
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

	glm:: mat4 matrix2 = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	texture::Texture fontAtlas{config::font_data::font.c_str()};
	graphics::FontMeshBuilder fontBuilder{config::font_data::fontLayout,
		config::font_data::fontAtlasWidth, config::font_data::fontArlasHeight};

	mesh::MeshElement mesh = fontBuilder.buldMeshForString("Hello World", 50);

	program.bind();

	glActiveTexture(GL_TEXTURE0);
	fontAtlas.bind();
	program.setUniformli("texture1", 0);

	program.setUniformMatrix4f("projection", matrix2);
	program.unbind();

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();
		glfwPollEvents();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClear(GL_COLOR_BUFFER_BIT);

		program.bind();

		mesh.draw();
		program.unbind();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

} /* namespace demo */
