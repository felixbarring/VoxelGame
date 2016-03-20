
#include "spriteDemo.h"

#include <string>
#include <iostream>
#include <memory>

#include <GL/glew.h>

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
#include "../util/fpsManager.h"

#include "../config/data.h"
#include "../graphics/resources.h"

#include <SFML/Window.hpp>

using namespace sf;

namespace demo {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

void SpriteDemo::runDemo()
{

	util::FPSManager fpsManager(60);
	const GLuint WIDTH = 800, HEIGHT = 600;

	config::graphics_data::windowWidth = WIDTH;
	config::graphics_data::windowHeight = HEIGHT;

	// create the window
	ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 1;

	Window window(VideoMode(800, 600), "Voxel Game", Style::Default,
			settings);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW\n";
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	std::shared_ptr<graphics::Sprite> sprite(new graphics::Sprite{
		200, 200, 0, 200, 200, graphics::Resources::getInstance().getTexture(
				config::texture_paths::scout)});
	std::shared_ptr<graphics::Sprite> sprite2(new graphics::Sprite{
		300, 300, 0, 200, 200, graphics::Resources::getInstance().getTexture(
				config::texture_paths::scout)});
	std::shared_ptr<graphics::Sprite> sprite3(new graphics::Sprite{
		600, 200, 0, 200, 200, graphics::Resources::getInstance().getTexture(
				config::texture_paths::scout)});

	while (window.isOpen())	{

		fpsManager.frameStart();

		glClear(GL_COLOR_BUFFER_BIT);

		graphics::SpriteBatcher::getInstance().addBatch(sprite);
		graphics::SpriteBatcher::getInstance().addBatch(sprite2);
		graphics::SpriteBatcher::getInstance().addBatch(sprite3);

		graphics::SpriteBatcher::getInstance().draw();

		fpsManager.sync();
		window.display();

        sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();
	}

}

} /* namespace graphics */
