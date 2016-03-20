
#include "textureDemo.h"

#include <GL/glew.h>

#include <map>

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

#include "../graphics/shaderProgram.h"
#include "../graphics/texture/texture.h"
#include "../graphics/mesh/meshElement.h"

#include "../config/data.h"

#include "../util/fpsManager.h"

#include <SFML/Window.hpp>

using namespace sf;

namespace demo
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

void TextureDemo::runDemo()
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
	if (glewInit() != GLEW_OK)
		std::cout << "Failed to initialize GLEW\n";

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	const char *vertex =
		"#version 330 core \n"
		"in vec2 positionIn; \n"
		"in vec2 texCoordIn; \n"

		"out vec2 texCoord; \n"

		"void main() \n"
		"{ \n"
		"  gl_Position = vec4(positionIn, 0.0f, 1.0f); \n"
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
		-0.4f, -0.4f,
		0.4f, -0.4f,
		0.4f, 0.4f,
		-0.4f, 0.4f,
	};

	std::vector<GLfloat> texCoords = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	std::vector<GLshort> indices = {
		0, 1, 3,
		1, 2, 3
	};

	mesh::MeshElement mesh{vertices, 2, texCoords, 2, indices};
	texture::Texture texture(config::texture_paths::scout.c_str());

	while (window.isOpen())	{

		fpsManager.frameStart();

		glClear(GL_COLOR_BUFFER_BIT);

		program.bind();

		glActiveTexture(GL_TEXTURE0);
		texture.bind();
		program.setUniformli("texture1", 0);

		mesh.draw();

		fpsManager.sync();
		window.display();

        sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

	}

}

}

