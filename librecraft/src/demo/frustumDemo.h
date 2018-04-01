
#ifndef SOURCE_DIRECTORY__SRC_DEMO_FRUSTUMDEMO_H_
#define SOURCE_DIRECTORY__SRC_DEMO_FRUSTUMDEMO_H_

#include <cmath>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/camera.h"
#include "../graphics/frustum.h"
#include "../graphics/mesh/meshElement.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/viewDirection.h"
#include "../util/fpsManager.h"
#include "../util/input.h"

#include <SFML/Window.hpp>

using namespace util;
using namespace sf;
using namespace std;

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

    Window window(
      VideoMode(800, 600), "Frustum Demo", Style::Default, settings);

    Input::createInstance(width / 2.0, height / 2.0);
    Input::getInstance()->setWindow(&window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
      std::cout << "Failed to initialize GLEW\n";

    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

    graphics::Camera camera{0, 0, 0};
    graphics::ViewDirection viewDirection;

    float size{1.0};

    // clang-format off

    vector<GLfloat> vertexData{
      // Front
      0, 0, size, // 0
      size, 0, size, // 1
      size, size, size, // 2
      0, size, size, // 3

      // Back
      size, 0, 0, // 0
      0, 0, 0, // 1
      0, size, 0, // 2
      size, size, 0, // 3

      // Right
      size, 0, size, // 0
      size, 0, 0, // 1
      size, size, 0, // 2
      size, size, size, // 3

      // Left
      0, 0, 0, // 0
      0, 0, size, // 1
      0, size, size, // 2
      0, size, 0, // 3

      // Top
      0, size, size, // 0
      size, size, size, // 1
      size, size, 0, // 2
      0, size, 0, // 3

      // Bottom
      0, 0, 0, // 0
      size, 0, 0, // 1
      size, 0, size, // 2
      0, 0, size, // 3

    };

    vector<short> elementData{
      0,      1,      2,      0,      2,      3,      0 + 4,  1 + 4,  2 + 4,
      0 + 4,  2 + 4,  3 + 4,  0 + 8,  1 + 8,  2 + 8,  0 + 8,  2 + 8,  3 + 8,
      0 + 12, 1 + 12, 2 + 12, 0 + 12, 2 + 12, 3 + 12, 0 + 16, 1 + 16, 2 + 16,
      0 + 16, 2 + 16, 3 + 16, 0 + 20, 1 + 20, 2 + 20, 0 + 20, 2 + 20, 3 + 20,
    };

    vector<pair<vector<float>, int>> vobs{{vertexData, 3}};
    unique_ptr<mesh::MeshElement> mesh{
      make_unique<mesh::MeshElement>(move(vobs), elementData)};

    string vertex =
        "#version 330 core \n"
        "in vec3 positionIn; \n"
        "uniform mat4 mvp; \n"
        "void main() { \n"
        "  gl_Position = mvp * vec4(positionIn, 1.0); \n"
        "} \n";

    string fragment =
        "#version 330 core \n"
        "out vec4 color; \n"
        "void main() { \n"
        "  color = vec4(1.0, 0.0, 0.0, 1.0); \n"
        "} \n";

    // clang-format on

    map<string, int> attributesMap{{"positionIn", 0}};
    graphics::ShaderProgram program{vertex, fragment, move(attributesMap)};

    constexpr double x{3.0};
    constexpr double y{0.0};
    constexpr double z{5.0};
    graphics::Transform transform{x, y, z};

    int counter{0};
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

      glm::mat4 modelView = camera.getViewMatrix() * transform.getMatrix();
      glm::mat4 modelViewProjection = camera.getProjectionMatrix() * modelView;
      program.setUniformMatrix4f("mvp", modelViewProjection);

      graphics::Frustum frustum{};
      frustum.update(modelViewProjection);

      if (!frustum.intersects(
            glm::vec3{x - 1, y - 1, z - 1},
            glm::vec3{x + size + 1, y + size + 1, z + size + 1})) {
        ++counter;
        cout << "Cube is not in the frustum! " << counter << "\n";
      }

      mesh->draw();

      fpsManager.sync();
      window.display();
    }
  }
};
}

#endif /* SOURCE_DIRECTORY__SRC_DEMO_FRUSTUMDEMO_H_ */
