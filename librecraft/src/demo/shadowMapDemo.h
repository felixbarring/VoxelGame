
#ifndef SRC_DEMO_SHADOWMAPDEMO_H_
#define SRC_DEMO_SHADOWMAPDEMO_H_

#include "iDemo.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "../graphics/resources.h"

using graphics::Camera;
using namespace graphics;

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


        // In
        const string positionIn = "positionIn";
        const string normalIn = "normalIn";
        const string texCoordIn = "texCoordIn";

        // Uniform
        const string lightValue = "lightValue";
        const string sunStrength = "sunStrenght";
        const string mvp = "modelViewProjection";

        const string arrayTexture = "arrayTexture";

        // Out
        const string faceNormalOut = "faceNormal";
        const string texCoordOut = "texCoord";
        const string lightOut = "light";

        const string colorOut = "color";

        string vertex =
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

        string fragment =
            "#version 330 core \n"

            "in vec3 " + texCoordOut + "; \n"

            "uniform sampler2DArray " + arrayTexture + "; \n"

            "out vec4 " + colorOut + "; \n"

            "void main(){ \n"
            "  " + colorOut + " = texture(" + arrayTexture + ", " + texCoordOut + "); \n"
            "  " + colorOut + ".w = 1.0; \n"
            "} \n";

        map<string, int> attributesMap{
            pair<string, int>(positionIn, 0),
            pair<string, int>(normalIn, 1),
            pair<string, int>(texCoordIn, 2)
        };

        std::unique_ptr<ShaderProgram> simpleProgram = make_unique<ShaderProgram>(vertex.c_str(), fragment.c_str(),
            attributesMap);

        string shadowDepthVert =
            "#version 330 core \n"

            "in vec3 " + positionIn + "; \n"
            "in vec3 " + normalIn + "; \n"
            "in vec3 " + texCoordIn + "; \n"

            "uniform mat4 " + mvp + "; \n"

            "void main() \n"
            "{ \n"
            "  gl_Position = " + mvp + " * vec4(" + positionIn + ", 1); \n"
            "} \n";

        string shadowDepthFrag =
            "#version 330 core \n"

            "out vec4 " + colorOut + "; \n"
            "void main() { \n"
            "  " + colorOut + " = vec4(gl_FragCoord.z); \n"
            "} \n";

        std::unique_ptr<ShaderProgram> shadowDepth = make_unique<ShaderProgram>(shadowDepthVert.c_str(), shadowDepthFrag.c_str(),
                attributesMap);

        texture::TextureArray texture{graphics::Resources::getInstance().getTextureArray(
            config::cube_data::textures,
            config::cube_data::TEXTURE_WIDTH,
            config::cube_data::TEXTURE_HEIGHT)};

        float aspectRatio = WIDTH / HEIGHT;
        glm::mat4 projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

        glm::mat4 cameraMatrix = glm::lookAt(
            glm::vec3(0, 0.1, 2), // Camera location
            glm::vec3(0, 0, 0),   // Look at
            glm::vec3(0, 0, 1)    // Head is up
        );

        Camera sceneCamera{};
        sceneCamera.setViewMatrix(std::move(cameraMatrix));
        sceneCamera.setProjectionMatrix(std::move(projection));


        glm::vec3 lightCameraLocation{0, 10, 0};
        Camera lightCamera{lightCameraLocation};
        const float dimension = 5.0f;
        glm::mat4 kek = glm::ortho(-dimension, dimension, -dimension, dimension, 0.01f, 20.0f);
        lightCamera.setProjectionMatrix(kek);

        int someCubeType{1}; // TODO replace with constant
        TexturedCube cube1{0.0, 0.0, 0.0, someCubeType};
        graphics::Transform transform1{0, 0, -1.0f};

        float size = 5.0f;

        vector<GLfloat> vertexData {
            -size, size, size, // 0
            size, size, size, // 1
            size, size, -size, // 2
            -size, size, -size, // 3
        };

        vector<GLfloat> normals {
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
        };

        GLfloat sideTexture = 1.0f; //BLOCK_TEXTURES[id][SIDE_TEXTURE];
        GLfloat topTexture = 1.0f; //BLOCK_TEXTURES[id][TOP_TEXTURE];
        GLfloat bottomTexture = 1.0f; //BLOCK_TEXTURES[id][BOTTOM_TEXTURE];

        vector<GLfloat> UV {
            0.0f, 0.0f, topTexture,
            1.0f, 0.0f, topTexture,
            1.0f, 1.0f, topTexture,
            0.0f, 1.0f, topTexture,

        };

        vector<short> elementData{
            0, 1, 2, 0, 2, 3,
        };

        std::shared_ptr<mesh::MeshElement> mesh;
        mesh.reset(new mesh::MeshElement(vertexData, 3, normals, 3, UV, 3, elementData));
        graphics::Transform floorTransform{0, -7, 0};
        graphics::ViewDirection viewDirection;


        // Create framebuffer and texture to draw to
        GLuint shadowFB;
        glGenFramebuffers(1, &shadowFB);

        GLuint depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glBindFramebuffer(GL_FRAMEBUFFER, shadowFB);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        while (window.isOpen()) {
            fpsManager.frameStart();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shadowDepth->bind();

            glEnable (GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);

            Input::getInstance()->updateValues();

            if (Input::getInstance()->escapeKeyPressed)
                break;

//            lightCamera.updateView(lightCameraLocation, viewDirection.getViewDirection(), viewDirection.getUpDirection());
            lightCamera.updateView(lightCameraLocation, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

            glActiveTexture(GL_TEXTURE0);
//            simpleProgram->setUniformli(arrayTexture, 0);
            texture.bind();

            glm::mat4 modelView = lightCamera.getViewMatrix() * floorTransform.getMatrix();
            glm::mat4 modelViewProjection = lightCamera.getProjectionMatrix() * modelView;

//            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

            shadowDepth->setUniformMatrix4f(mvp, modelViewProjection);
            mesh->draw();



            // TODO Draw with the from the player cameras perspective.
            viewDirection.changeViewDirection(Input::getInstance()->mouseXMovement, Input::getInstance()->mouseYMovement);



            transform1.rotateX(0.1);
            modelView = lightCamera.getViewMatrix() * transform1.getMatrix();
            modelViewProjection = lightCamera.getProjectionMatrix() * modelView;

            shadowDepth->setUniformMatrix4f(mvp, modelViewProjection);
            cube1.draw();

            shadowDepth->unbind();

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
