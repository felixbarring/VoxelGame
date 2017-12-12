
#ifndef SRC_DEMO_SPRITEDEMO_H_
#define SRC_DEMO_SPRITEDEMO_H_

#include "iDemo.h"

#include <iostream>
#include <memory>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/camera.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/sprite.h"
#include "../graphics/viewDirection.h"

#include "../graphics/spriteBatcher.h"
#include "../graphics/texture/texture.h"

#include "../graphics/cubeMap.h"
#include "../graphics/mesh/meshElement.h"
#include "../util/fpsManager.h"

#include "../config/data.h"
#include "../graphics/resources.h"

#include <SFML/Window.hpp>

#include "../graphics/graphicsManager.h"
namespace demo {

class SpriteDemo : public IDemo
{
public:
  void runDemo() override
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

    Window window(VideoMode(800, 600), "Voxel Game", Style::Default, settings);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
      std::cout << "Failed to initialize GLEW\n";
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

    std::shared_ptr<graphics::Sprite> sprite(
      new graphics::Sprite{ 200,
                            200,
                            0,
                            200,
                            200,
                            graphics::Resources::getInstance().getTexture(
                              config::cube_data::textures[0]) });

    std::shared_ptr<graphics::Sprite> sprite2(
      new graphics::Sprite{ 300,
                            300,
                            0,
                            200,
                            200,
                            graphics::Resources::getInstance().getTexture(
                              config::cube_data::textures[1]) });

    std::shared_ptr<graphics::Sprite> sprite3(
      new graphics::Sprite{ 600,
                            200,
                            0,
                            200,
                            200,
                            graphics::Resources::getInstance().getTexture(
                              config::cube_data::textures[2]) });

    graphics::GraphicsManager graphicsmanager{};

    while (window.isOpen()) {

      fpsManager.frameStart();

      glClear(GL_COLOR_BUFFER_BIT);

      graphicsmanager.getSpriteBatcher().addBatch(*sprite);
      graphicsmanager.getSpriteBatcher().addBatch(*sprite2);
      graphicsmanager.getSpriteBatcher().addBatch(*sprite3);

      graphicsmanager.getSpriteBatcher().draw();

      fpsManager.sync();
      window.display();

      sf::Event event;
      while (window.pollEvent(event))
        if (event.type == sf::Event::Closed)
          window.close();
    }
  }
};

} /* namespace demo */

#endif /* SRC_DEMO_SPRITEDEMO_H_ */
