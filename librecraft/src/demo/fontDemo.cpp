
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

#include "../graphics/sprite.h"
#include "../graphics/spriteBatcher.h"
#include "../graphics/texture/texture.h"
#include "../graphics/fontMeshBuilder.h"
#include "../graphics/mesh/meshElement.h"
#include "../graphics/shaderProgram.h"


namespace demo {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

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

	texture::Texture fontAtlas{config::font_data::font.c_str()};
	graphics::FontMeshBuilder fontBuilder{config::font_data::fontLayout,
		config::font_data::fontAtlasWidth, config::font_data::fontAtlasHeight};

	std::shared_ptr<mesh::MeshElement> mesh = fontBuilder.buldMeshForString("Hello World", 50);
	std::shared_ptr<graphics::Sprite> sprite(new graphics::Sprite(0, 0, 0, mesh, fontAtlas));

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();
		glfwPollEvents();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClear(GL_COLOR_BUFFER_BIT);

		graphics::SpriteBatcher::getInstance().addBatch(sprite);
		graphics::SpriteBatcher::getInstance().draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

} /* namespace demo */
