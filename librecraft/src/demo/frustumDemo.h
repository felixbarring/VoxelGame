
#ifndef SOURCE_DIRECTORY__SRC_DEMO_FRUSTUMDEMO_H_
#define SOURCE_DIRECTORY__SRC_DEMO_FRUSTUMDEMO_H_

#include <iostream>

#include "../../include/glm/detail/type_mat.hpp"
#include "../graphics/frustum.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/camera.h"
#include "../graphics/cubeMap.h"
#include "../graphics/resources.h"
#include "../graphics/texture/textureCubeMap.h"
#include "../graphics/viewDirection.h"
#include "../util/fpsManager.h"
#include "../util/input.h"

#include <SFML/Window.hpp>

using namespace util;
using namespace sf;

namespace demo {

class FrustumDemo {
public:

  void runDemo() {

    FPSManager fpsManager(100);
    const GLuint width{800};
    const GLuint height{600};

    config::graphics_data::windowWidth = width;
    config::graphics_data::windowHeight = height;

    // create the window
    ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 1;

    Window window(VideoMode(800, 600), "Frustum Demo", Style::Default, settings);

    Input::createInstance(width / 2.0, height / 2.0);
    Input::getInstance()->setWindow(&window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
      std::cout << "Failed to initialize GLEW\n";

    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

    graphics::Camera camera{0, 0, 0};
    graphics::ViewDirection viewDirection;


    // Create A few meshes of different sizes

    // Render them

    // Make a bounding volume for them

    // Use frustum to check if they should be rendered or not.

    Frustum frustum{glm::mat4()}; // TODO Use the mvp matrix here...

    while (window.isOpen()) {
      fpsManager.frameStart();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      Input::getInstance()->updateValues();

      if (Input::getInstance()->escapeKeyPressed)
        window.close();

      viewDirection.changeViewDirection(Input::getInstance()->mouseXMovement,
                                        Input::getInstance()->mouseYMovement);
      camera.updateView(glm::vec3(0, 0, 0),
                        viewDirection.getViewDirection(),
                        viewDirection.getUpDirection());

      fpsManager.sync();
      window.display();
    }

  }

};

}

#endif /* SOURCE_DIRECTORY__SRC_DEMO_FRUSTUMDEMO_H_ */
