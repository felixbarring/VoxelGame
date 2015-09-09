
#include "spriteDemo.h"

#include <string>
#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shaderProgram.h"
#include "../graphics/camera.h"
#include "../graphics/viewDirection.h"
#include "../graphics/sprite.h"

#include "../graphics/texture/texture.h"
#include "../graphics/spriteBatcher.h"

#include "../graphics/mesh/meshElement.h"
#include "../graphics/cubeMap.h"
#include "../graphics/textureResources.h"
#include "../util/fpsManager.h"

#include "../config/data.h"

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

	std::shared_ptr<graphics::Sprite> sprite(new graphics::Sprite{200, 200, 0, 200, 200, graphics::TextureResources::getInstance().getTexture(config::texture_paths::scout)});
	std::shared_ptr<graphics::Sprite> sprite2(new graphics::Sprite{300, 300, 0, 200, 200, graphics::TextureResources::getInstance().getTexture(config::texture_paths::scout)});
	std::shared_ptr<graphics::Sprite> sprite3(new graphics::Sprite{600, 200, 0, 200, 200, graphics::TextureResources::getInstance().getTexture(config::texture_paths::scout)});

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		graphics::SpriteBatcher::getInstance().addBatch(sprite);
		graphics::SpriteBatcher::getInstance().addBatch(sprite2);
		graphics::SpriteBatcher::getInstance().addBatch(sprite3);

		graphics::SpriteBatcher::getInstance().draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

} /* namespace graphics */
