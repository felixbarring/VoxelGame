#include "cubeBatcher.h"

#include <string>
#include <map>
#include <iostream>

#include "shaderProgram.h"
#include "../config/data.h"
#include "resources.h"

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

CubeBatcher::CubeBatcher(Camera &camera)
    : m_texture(
            graphics::Resources::getInstance().getTextureArray(
                    config::cube_data::textures,
                    config::cube_data::TEXTURE_WIDTH,
                    config::cube_data::TEXTURE_HEIGHT)),
      m_camera(camera)
{

    for (int i = 0; i <= config::cube_data::LAST_CUBE + 1; i++) {
        m_cubes.push_back(TexturedCube {2, 0, -1.0f, i});
    }


    const char *vertex =
        "#version 330 core \n"

        "in vec3 positionIn; \n"
        "in vec3 normalIn; \n"
        "in vec3 texCoordIn; \n"

        "uniform float lightValue; \n"
        "uniform float sunStrenght; \n"

        "uniform mat4 modelViewProjection; \n"

        "out vec3 faceNormal; \n"
        "out vec3 texCoord; \n"
        "out float light; \n"

        "void main(){ \n"
        "  texCoord = vec3(texCoordIn.x, texCoordIn.y, texCoordIn.z); \n"
        "  light = (lightValue / 16) * sunStrenght; \n"
        "  gl_Position =  modelViewProjection * vec4(positionIn, 1); \n"
        "} \n";

    const char *fragment =
        "#version 330 core \n"

        "in vec3 texCoord; \n"
        "in float light; \n"

        "uniform sampler2DArray texture1; \n"

        "out vec4 color; \n"

        "void main(){ \n"
        "  color = light * texture(texture1, texCoord); \n"
        "  color.w = 1.0; \n"
        "} \n";

    std::map<std::string, int> attributesMap{
        std::pair<std::string, int>("positionIn", 0),
        std::pair<std::string, int>("normalIn", 1),
        std::pair<std::string, int>("texCoordIn", 2)
    };

    m_program.reset(new ShaderProgram{vertex, fragment, attributesMap});

}

// ########################################################
// Member Functions########################################
// ########################################################

void CubeBatcher::addBatch(char type, Transform &transform, int lightValue) {
    m_batches.push_back(Batch(m_cubes.at(type), transform, lightValue));
}

void CubeBatcher::draw() {

    m_program->bind();

    glEnable (GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    m_program->setUniformli("texture1", 0);
    m_texture.bind();

    m_program->setUniform1f("sunStrenght", m_sunStrength);

    for (auto b : m_batches) {
        m_program->setUniform1f("lightValue", b.m_lightValue);

        glm::mat4 modelView = m_camera.getViewMatrix() * b.m_transform.getMatrix();
        glm::mat4 modelViewProjection = m_camera.getProjectionMatrix() * modelView;
        m_program->setUniformMatrix4f("modelViewProjection", modelViewProjection);
        b.m_cube.draw();
    }

    m_program->unbind();
    m_batches.clear();
}

void CubeBatcher::setSunStrenght(float value) {
    m_sunStrength = value;
}

} /* namespace graphics */
