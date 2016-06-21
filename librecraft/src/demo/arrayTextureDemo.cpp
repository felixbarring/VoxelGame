
#include "arrayTextureDemo.h"

#include <GL/glew.h>

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
#include "../util/input.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

namespace demo
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Constructor/Destructor #################################
// ########################################################

void ArrayTextureDemo::runDemo()
{
	util::FPSManager fpsManager(60);

	int WIDTH = 800;
	int HEIGHT = 600;

	util::Input::createInstance(WIDTH / 2.0, HEIGHT / 2.0);

	config::graphics_data::windowWidth = WIDTH;
	config::graphics_data::windowHeight = HEIGHT;

    // create the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 1;

	sf::Window window(sf::VideoMode(800, 600), "Array Texture Demo",
			sf::Style::Default, settings);

	util::Input::getInstance()->setWindow(&window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		std::cout << "Failed to initialize GLEW\n";

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

	texture::TextureArray texture{
		config::cube_data::textures,
		config::cube_data::TEXTURE_WIDTH,
		config::cube_data::TEXTURE_HEIGHT};

	while (window.isOpen()) {

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
