
#ifndef SRC_DEMO_CUBEMAPDEMO_H_
#define SRC_DEMO_CUBEMAPDEMO_H_

#include "iDemo.h"

#include <iostream>
#include <string>

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

using graphics::Camera;

using namespace util;
using namespace sf;

namespace demo {

class CubeMapDemo : public IDemo {
public:
  void runDemo() override {
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

    Window window(VideoMode(800, 600), "Voxel Game", Style::Default, settings);

    Input::createInstance(WIDTH / 2.0, HEIGHT / 2.0);
    Input::getInstance()->setWindow(&window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
      std::cout << "Failed to initialize GLEW\n";

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

    Camera camera{0, 0, 0};

    texture::TextureCubeMap& texture =
      graphics::Resources::getInstance().getTextureCubeMap(
        config::cube_map_data::cubeMap1[0],
        config::cube_map_data::cubeMap1[1],
        config::cube_map_data::cubeMap1[2],
        config::cube_map_data::cubeMap1[3],
        config::cube_map_data::cubeMap1[4],
        config::cube_map_data::cubeMap1[5]);

    graphics::CubeMap skybox{texture, camera};

    graphics::ViewDirection viewDirection;

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

      skybox.draw(1.0);

      fpsManager.sync();
      window.display();
    }
  }
};
}

#endif /* SRC_DEMO_CUBEMAPDEMO_H_ */
