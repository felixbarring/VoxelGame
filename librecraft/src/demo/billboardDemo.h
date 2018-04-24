#ifndef SRC_DEMO_BILLBOARDDEMO_H_
#define SRC_DEMO_BILLBOARDDEMO_H_

#include <SFML/Graphics/Text.hpp>
#include <iostream>

#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <utility>
#include <vector>

#include "../graphics/camera.h"
#include "../graphics/viewDirection.h"
#include "../util/fpsManager.h"
#include "../util/input.h"

#include "../graphics/camera.h"
#include "../graphics/frustum.h"
#include "../graphics/mesh/meshElement.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/transform.h"
#include "../graphics/viewDirection.h"
#include "../util/fpsManager.h"
#include "../util/input.h"

using namespace sf;
using util::FPSManager;
using util::Input;

namespace demo {

class BillboardDemo {
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

    Window window(
      VideoMode(800, 600), "Billboard Demo", Style::Default, settings);

    Input::createInstance(width / 2.0, height / 2.0);
    Input::getInstance()->setWindow(&window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
      std::cout << "Failed to initialize GLEW\n";

    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

    graphics::Camera camera{0, 0, 0};
    graphics::ViewDirection viewDirection;

    // clang-format off

    std::vector<GLfloat> vertexData{
      0, 0, 1, // 0
      1, 0, 1, // 1
      1, 1, 1, // 2
      0, 1, 1, // 3
    };

    std::vector<short> elementData{
      0, 1, 2,
      0, 2, 3
    };

    std::string vertex =
        "#version 330 core \n"
        "in vec3 positionIn; \n"
        "uniform mat4 mvp; \n"
        "void main() { \n"
        "  gl_Position = mvp * vec4(positionIn, 1.0); \n"
        "} \n";

    std::string fragment =
        "#version 330 core \n"
        "out vec4 color; \n"
        "void main() { \n"
        "  color = vec4(0.0, 1.0, 0.0, 1.0); \n"
        "} \n";

    // clang-format on

    std::map<std::string, int> attributesMap{{"positionIn", 0}};
    graphics::ShaderProgram program{vertex, fragment, std::move(attributesMap)};

    std::vector<std::pair<std::vector<float>, int>> vobs{{vertexData, 3}};
    std::unique_ptr<mesh::MeshElement> mesh{
      std::make_unique<mesh::MeshElement>(std::move(vobs), elementData)};

    graphics::Transform transform{0, 1, -5};

    // TODO Render a simple plane with a color.
    while (window.isOpen()) {
      fpsManager.frameStart();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      Input::getInstance()->updateValues();

      if (Input::getInstance()->escapeKeyPressed)
        window.close();

      viewDirection.changeViewDirection(Input::getInstance()->mouseXMovement,
                                        Input::getInstance()->mouseYMovement);

      camera.updateView(glm::vec3(0, 0, 0),
                        viewDirection.getViewDirection(),
                        viewDirection.getUpDirection());

      program.bind();

      glm::mat4 modelView = camera.getViewMatrix() * transform.getMatrix();
      glm::mat4 modelViewProjection = camera.getProjectionMatrix() * modelView;
      program.setUniformMatrix4f("mvp", modelViewProjection);

      mesh->draw();

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

#endif /* SRC_DEMO_BILLBOARDDEMO_H_ */
