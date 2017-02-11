
#ifndef SRC_DEMO_SHADOWMAPDEMO_H_
#define SRC_DEMO_SHADOWMAPDEMO_H_

#include "iDemo.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using graphics::Camera;

#include "../graphics/camera.h"
namespace demo {

class ShadowMapDemo : public IDemo {
public:

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

        float aspectRatio = WIDTH / HEIGHT;
        glm::mat4 projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

        glm::mat4 cameraMatrix = glm::lookAt(
            glm::vec3(0, 0.1, 2), // Camera location
            glm::vec3(0, 0, 0),   // Look at
            glm::vec3(0, 0, 1)    // Head is up
        );

        float xAmount = 0;

        Camera sceneCamera{};
        sceneCamera.setViewMatrix(std::move(cameraMatrix));
        sceneCamera.setProjectionMatrix(std::move(projection));

//        graphics::CubeBatcher batcher{sceneCamera};

        // TODO Use :p
        Camera lightCamera{};

        while (window.isOpen()) {
           fpsManager.frameStart();

           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//           batcher.draw();

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

#endif /* SRC_DEMO_SHADOWMAPDEMO_H_ */
