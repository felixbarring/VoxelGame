#include "cubeMapDemo.h"

#include <string>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/camera.h"
#include "../graphics/viewDirection.h"
#include "../graphics/texture/textureCubeMap.h"
#include "../util/fpsManager.h"
#include "../graphics/cubeMap.h"
#include "../graphics/resources.h"
#include "../util/input.h"

#include <SFML/Window.hpp>

using namespace util;
using namespace sf;

namespace demo {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

void CubeMapDemo::runDemo() {
	FPSManager fpsManager(100);
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

	Input::createInstance(nullptr, WIDTH / 2.0, HEIGHT / 2.0);
	Input::getInstance()->setWindow(&window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		std::cout << "Failed to initialize GLEW\n";

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	graphics::Camera &camera = graphics::Camera::getInstance();

	texture::TextureCubeMap &texture =
			graphics::Resources::getInstance().getTextureCubeMap(
					config::cube_map_data::cubeMap1[0],
					config::cube_map_data::cubeMap1[1],
					config::cube_map_data::cubeMap1[2],
					config::cube_map_data::cubeMap1[3],
					config::cube_map_data::cubeMap1[4],
					config::cube_map_data::cubeMap1[5],
					config::cube_map_data::cubeMap1Width,
					config::cube_map_data::cubeMap1Height);

	graphics::CubeMap skybox {texture};

	float screenCenterX = WIDTH / 2;
	float screenCenterY = HEIGHT / 2;

	graphics::ViewDirection viewDirection;

	while (window.isOpen()) {

		fpsManager.frameStart();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LEQUAL);

		Input::getInstance()->updateValues();

		if (Input::getInstance()->escapeKeyPressed)
			window.close();

		viewDirection.changeViewDirection(
				Input::getInstance()->mouseXMovement,
				Input::getInstance()->mouseYMovement);
		camera.updateView(glm::vec3(0, 0, 0), viewDirection.getViewDirection(),
				viewDirection.getUpDirection());

		skybox.render();

		fpsManager.sync();
		window.display();
	}

}

}

