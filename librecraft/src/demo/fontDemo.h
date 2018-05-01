
#ifndef SRC_DEMO_FONTDEMO_H_
#define SRC_DEMO_FONTDEMO_H_

#include <iostream>
#include <map>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "config/data.h"
#include "graphics/fontMeshBuilder.h"
#include "graphics/mesh/meshElement.h"
#include "graphics/shaderProgram.h"
#include "graphics/sprite.h"
#include "graphics/spriteBatcher.h"
#include "graphics/texture/texture.h"
#include "util/fpsManager.h"

#include <SFML/Window.hpp>

#include "../graphics/graphicsManager.h"
using namespace sf;

namespace demo {

class FontDemo {
public:
  void runDemo() {
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
    if (glewInit() != GLEW_OK)
      std::cout << "Failed to initialize GLEW\n";

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

    texture::Texture fontAtlas{config::font_data::font.c_str()};
    graphics::FontMeshBuilder fontBuilder{config::font_data::fontLayout,
                                          config::font_data::fontAtlasWidth,
                                          config::font_data::fontAtlasHeight};

    std::shared_ptr<mesh::MeshElement> mesh =
      fontBuilder.buldMeshForString("Hello World", 50);
    std::unique_ptr<graphics::Sprite> sprite =
      std::make_unique<graphics::Sprite>(0, 0, 0, mesh, fontAtlas);

    graphics::GraphicsManager graphicsManager{};

    while (window.isOpen()) {

      fpsManager.frameStart();

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glClear(GL_COLOR_BUFFER_BIT);

      graphicsManager.getSpriteBatcher().addBatch(*sprite);
      graphicsManager.getSpriteBatcher().draw();

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

#endif /* SRC_DEMO_FONTDEMO_H_ */
