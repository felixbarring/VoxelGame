
#ifndef SRC_DEMO_CUBEDEMO_H_
#define SRC_DEMO_CUBEDEMO_H_

#include "iDemo.h"

#include <GL/glew.h>

#include <map>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shaderProgram.h"
#include "../graphics/texture/textureArray.h"
#include "../graphics/transform.h"
#include "../graphics/texturedCube.h"
#include "../graphics/cubeBatcher.h"
#include "../graphics/camera.h"
#include "../graphics/viewDirection.h"
#include "../util/fpsManager.h"
#include "../util/input.h"
#include "../config/data.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

using graphics::Camera;

namespace demo
{

class CubeDemo : public IDemo {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

	void runDemo() override {

	    util::FPSManager fpsManager(100);
	    int WIDTH = 800;
	    int HEIGHT = 600;

	    util::Input::createInstance(WIDTH / 2.0, HEIGHT / 2.0);

	    // create the window
	    sf::ContextSettings settings;
	    settings.depthBits = 24;
	    settings.stencilBits = 8;
	    settings.antialiasingLevel = 4;
	    settings.majorVersion = 3;
	    settings.minorVersion = 1;

	    sf::Window window(sf::VideoMode(800, 600), "Cube Demo",
	            sf::Style::Default, settings);

	    util::Input::getInstance()->setWindow(&window);

	    glewExperimental = true;
	    if (glewInit() != GLEW_OK)
	        std::cout << "Failed to initialize GLEW\n";

	    glViewport(0, 0, WIDTH, HEIGHT);
	    glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	    glEnable(GL_DEPTH_TEST);
	    glDepthFunc(GL_LESS);

	    graphics::Transform transform1{0, 0, -5.0f};
	    graphics::Transform transform2{2, 0, -5.0f};
	    graphics::Transform transform3{4, 0, -5.0f};
	    graphics::Transform transform4{6, 0, -5.0f};

	    float aspectRatio = WIDTH / HEIGHT;
	    glm::mat4 Projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

	    glm::mat4 camera = glm::lookAt(
	        glm::vec3(0, 0.1, 2), // Camera location
	        glm::vec3(0, 0, 0),   // Look at
	        glm::vec3(0, 0, 1)    // Head is up
	    );

	    float xAmount = 0;

	    while (window.isOpen()) {
	        fpsManager.frameStart();

	        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	        transform1.rotateX(0.1);
	        transform2.rotateY(0.1);
	        transform3.rotateZ(0.1);
	        transform4.rotateX(0.1);
	        transform4.rotateY(0.1);
	        transform4.rotateZ(0.1);

	        // TODO Do not use the batcher in the demos?
	        // Or perhaps create a batcher not in the graphicsManager since it is no longer singleton :-)

	//      graphics::CubeBatcher::getInstance().addBatch(1, transform1);
	//      graphics::CubeBatcher::getInstance().addBatch(2, transform2);
	//      graphics::CubeBatcher::getInstance().addBatch(3, transform3);
	//      graphics::CubeBatcher::getInstance().addBatch(4, transform4);
	//
	//      graphics::CubeBatcher::getInstance().draw();

	        fpsManager.sync();
	        window.display();

	        sf::Event event;
	        while (window.pollEvent(event))
	            if (event.type == sf::Event::Closed)
	                window.close();
	    }

	}

};

}

#endif /* SRC_DEMO_CUBEDEMO_H_ */
