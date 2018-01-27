#ifndef SRC_DEMO_SHADOWMAPDEMO_H_
#define SRC_DEMO_SHADOWMAPDEMO_H_

#include "iDemo.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "../graphics/camera.h"
#include "../graphics/resources.h"

using graphics::Camera;
using namespace graphics;

namespace demo {

class ShadowMapDemo : public IDemo {
public:
  void runDemo() override {

    util::FPSManager fpsManager(100);
    const int width{800};
    const int height{600};

    util::Input::createInstance(width / 2.0, height / 2.0);

    // create the window
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 1;

    sf::Window window(
      sf::VideoMode(width, height), "Cube Demo", sf::Style::Default, settings);

    util::Input::getInstance()->setWindow(&window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
      std::cout << "Failed to initialize GLEW\n";

    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // In
    const std::string positionIn = "positionIn";
    const std::string normalIn = "normalIn";
    const std::string texCoordIn = "texCoordIn";

    // Uniform
    const std::string lightValue = "lightValue";
    const std::string sunStrength = "sunStrenght";
    const std::string mvp = "modelViewProjection";

    const string arrayTexture = "arrayTexture";

    // Out
    const std::string faceNormalOut = "faceNormal";
    const std::string texCoordOut = "texCoord";
    const std::string lightOut = "light";

    const std::string colorOut = "color";

    // clang-format off

    std::string vertex =
      "#version 330 core \n"
      "in vec3 " + positionIn + "; \n"
      "in vec3 " + normalIn + "; \n"
      "in vec3 " + texCoordIn + "; \n"
      "uniform mat4 " + mvp + "; \n"

      "out vec3 " + texCoordOut + "; \n"

      "void main(){ \n"
      "  " + texCoordOut + " = " + texCoordIn + "; \n"
      "  gl_Position = " + mvp + " * vec4(" + positionIn + ", 1); \n"
      "} \n";

    std::string fragment =
        "#version 330 core \n"

        "in vec3 " + texCoordOut + "; \n"
        "uniform sampler2DArray " + arrayTexture + "; \n"
        "out vec4 " + colorOut + "; \n"
        "void main(){ \n"
        "  " + colorOut + " = texture(" + arrayTexture + ", " +texCoordOut + "); \n"
        "  " + colorOut + ".w = 1.0; \n"
        "} \n";

    // clang-format on

    std::map<std::string, int> attributesMap{
      std::pair<std::string, int>(positionIn, 0),
      std::pair<std::string, int>(normalIn, 1),
      std::pair<std::string, int>(texCoordIn, 2)};

    std::unique_ptr<ShaderProgram> simpleProgram = make_unique<ShaderProgram>(
      vertex.c_str(), fragment.c_str(), attributesMap);

    // clang-format off

    std::string shadowDepthVert =
        "#version 330 core \n"

        "in vec3 " + positionIn + "; \n"
        "in vec3 " + normalIn + "; \n"
        "in vec3 " + texCoordIn + "; \n"

        "uniform mat4 " + mvp + "; \n"

        "void main() \n"
        "{ \n"
        "  gl_Position = " +mvp + " * vec4(" + positionIn + ", 1); \n"
        "} \n";

    std::string shadowDepthFrag =
        "#version 330 core \n"

        "out vec4 " + colorOut + "; \n"
        "void main() { \n"
        "  " + colorOut + " = vec4(gl_FragCoord.z); \n"
                                             "} \n";

    // clang-format on

    std::unique_ptr<ShaderProgram> shadowDepthProgram =
      std::make_unique<ShaderProgram>(
        shadowDepthVert.c_str(), shadowDepthFrag.c_str(), attributesMap);

    texture::TextureArray texture{
      graphics::Resources::getInstance().getTextureArray(
        config::cube_data::textures,
        config::cube_data::TEXTURE_WIDTH,
        config::cube_data::TEXTURE_HEIGHT)};

    float aspectRatio = width / height;
    glm::mat4 projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

    glm::vec3 sceneCameraLocation{0, 10, 0};
    Camera sceneCamera{};
    //    sceneCamera.setViewMatrix(std::move(cameraMatrix));
    sceneCamera.setProjectionMatrix(std::move(projection));

    glm::vec3 lightCameraLocation{0, 10, 0};
    Camera lightCamera{lightCameraLocation};
    const float dimension = 5.0f;
    glm::mat4 kek =
      glm::ortho(-dimension, dimension, -dimension, dimension, 0.01f, 20.0f);
    lightCamera.setProjectionMatrix(kek);
    lightCamera.updateView(lightCameraLocation,
                           glm::vec3(0.0f, -1.0f, 0.0f),
                           glm::vec3(0.0f, 0.0f, 1.0f));

    int someCubeType{1}; // TODO replace with constant
    TexturedCube cube1{/*0.0, 0.0, 0.0,*/ someCubeType};
    graphics::Transform transform1{0.0, 0.0, 0.0};

    float size{5.0f};

    // clang-format off

    std::vector<GLfloat> vertexData{
      -size, size, size,  // 0
      size,  size, size,  // 1
      size,  size, -size, // 2
      -size, size, -size, // 3
    };

    std::vector<GLfloat> normals{
      0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    };

    //    GLfloat sideTexture = 1.0f; //BLOCK_TEXTURES[id][SIDE_TEXTURE];
    GLfloat topTexture = 1.0f; // BLOCK_TEXTURES[id][TOP_TEXTURE];
    //    GLfloat bottomTexture = 1.0f; //BLOCK_TEXTURES[id][BOTTOM_TEXTURE];

    std::vector<GLfloat> UV{
      0.0f, 0.0f, topTexture, 1.0f, 0.0f, topTexture,
      1.0f, 1.0f, topTexture, 0.0f, 1.0f, topTexture,
    };

    std::vector<short> elementData{ 0, 1, 2, 0, 2, 3 };

    // clang-format on

    std::shared_ptr<mesh::MeshElement> mesh;

    std::vector<std::pair<std::vector<float>, int>> vbos{
      {vertexData, 3}, {normals, 3}, {UV, 3}};
    mesh.reset(new mesh::MeshElement(vbos, elementData));
    graphics::Transform floorTransform{0, -7, 0};
    graphics::ViewDirection viewDirection;

    // Create framebuffer and texture to draw to
    GLuint shadowFB;
    glGenFramebuffers(1, &shadowFB);

    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_DEPTH_COMPONENT,
                 width,
                 height,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_FLOAT,
                 nullptr);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFB);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (window.isOpen()) {

      Input::getInstance()->updateValues();
      Input::getInstance()->lockMouse();

      if (Input::getInstance()->escapeKeyActive)
        return;

      if (Input::getInstance()->moveForwardActive)
        sceneCameraLocation += viewDirection.getViewDirection();

      if (Input::getInstance()->moveBackwardActive)
        sceneCameraLocation -= viewDirection.getViewDirection();

      if (Input::getInstance()->moveLeftActive)
        sceneCameraLocation -= -viewDirection.getRightDirection();

      if (Input::getInstance()->moveRightActive)
        sceneCameraLocation -= viewDirection.getRightDirection();

      glm::mat4 modelView;
      glm::mat4 modelViewProjection;

      fpsManager.frameStart();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);

      shadowDepthProgram->bind();

      modelView = lightCamera.getViewMatrix() * floorTransform.getMatrix();
      modelViewProjection = lightCamera.getProjectionMatrix() * modelView;

      glBindFramebuffer(GL_FRAMEBUFFER, shadowFB);

      shadowDepthProgram->setUniformMatrix4f(mvp, modelViewProjection);

      mesh->draw();

      transform1.rotateX(0.01);
      modelView = lightCamera.getViewMatrix() * transform1.getMatrix();
      modelViewProjection = lightCamera.getProjectionMatrix() * modelView;

      shadowDepthProgram->setUniformMatrix4f(mvp, modelViewProjection);
      cube1.draw();

      shadowDepthProgram->unbind();

      //       TODO Draw with the from the player cameras perspective.

      simpleProgram->bind();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      glActiveTexture(GL_TEXTURE0);
      simpleProgram->setUniformli(arrayTexture, 0);
      texture.bind();

      viewDirection.setSensitivity(0.01, 0.01);

      viewDirection.changeViewDirection(Input::getInstance()->mouseXMovement,
                                        Input::getInstance()->mouseYMovement);

      // TODO - up direction is very wrong, fix it!
      sceneCamera.updateView(sceneCameraLocation,
                             viewDirection.getViewDirection(),
                             -viewDirection.getUpDirection());

      modelView = sceneCamera.getViewMatrix() * floorTransform.getMatrix();
      modelViewProjection = sceneCamera.getProjectionMatrix() * modelView;

      simpleProgram->setUniformMatrix4f(mvp, modelViewProjection);

      mesh->draw();

      modelView = sceneCamera.getViewMatrix() * transform1.getMatrix();
      modelViewProjection = sceneCamera.getProjectionMatrix() * modelView;

      simpleProgram->setUniformMatrix4f(mvp, modelViewProjection);

      cube1.draw();

      simpleProgram->unbind();

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
