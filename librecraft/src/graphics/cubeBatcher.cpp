#include "cubeBatcher.h"

#include <iostream>
#include <map>
#include <string>

#include "../config/data.h"
#include "resources.h"
#include "shaderProgram.h"

using namespace std;

// In
const string positionIn = "positionIn";
const string normalIn = "normalIn";
const string texCoordIn = "texCoordIn";

// Uniform
const string sunLight = "lightValue";
const string otherLight = "otherLight";
const string sunStrength = "sunStrenght";
const string mvp = "modelViewProjection";

const string arrayTexture = "arrayTexture";

// Out
const string faceNormalOut = "faceNormal";
const string texCoordOut = "texCoord";
const string lightOut = "light";

const string colorOut = "color";

graphics::ShaderProgram cubeBatcherCreateShader() {
  // clang-format off

  string vertex =
    "#version 330 core \n"

    "in vec3 " + positionIn + "; \n"
    "in vec3 " + normalIn + "; \n"
    "in vec3 " + texCoordIn + "; \n"

    "uniform float " + sunLight + "; \n"
    "uniform float " + otherLight + "; \n"
    "uniform float " + sunStrength + "; \n"
    "uniform mat4 " + mvp + "; \n"
    "out vec3 " + faceNormalOut + "; \n"
    "out vec3 " + texCoordOut + "; \n"
    "out float " + lightOut + "; \n"

    "void main(){ \n"
    "  " + texCoordOut + " = " + texCoordIn + "; \n"
    "  " + lightOut + " = max(" + sunLight + " * " + sunStrength + ", " + otherLight + ") / 16; \n"
    "  gl_Position = " + mvp + " * vec4(" + positionIn + ", 1); \n"
    "} \n";

  string fragment =
      "#version 330 core \n"

      "in vec3 " + texCoordOut + "; \n"
      "in float " + lightOut + "; \n"
      "uniform sampler2DArray " + arrayTexture + "; \n"
      "out vec4 " + colorOut + "; \n"

      "void main(){ \n"
      "  " + colorOut + " = " + lightOut + " * texture(" + arrayTexture + ", " + texCoordOut + "); \n"
      "  " + colorOut + ".w = 1.0; \n"
      "} \n";

  // clang-format on

  map<string, int> attributesMap{pair<string, int>(positionIn, 0),
                                 pair<string, int>(normalIn, 1),
                                 pair<string, int>(texCoordIn, 2)};

  return graphics::ShaderProgram(vertex, fragment, attributesMap);
}

namespace graphics {

CubeBatcher::CubeBatcher(Camera& camera)
  : m_camera(camera)
  , m_program{cubeBatcherCreateShader()}
  , m_texture(graphics::Resources::getInstance().getTextureArray(
      config::cube_data::textures,
      config::cube_data::TEXTURE_WIDTH,
      config::cube_data::TEXTURE_HEIGHT)) {

  for (int i = 0; i <= config::cube_data::LAST_CUBE + 1; i++)
    m_cubes.push_back(TexturedCube{/*2, 0, -1.0f, */ i});
}

void
CubeBatcher::addBatch(char type,
                      Transform& transform,
                      int sunLight,
                      int otherLight) {
  m_batches.push_back(Batch(m_cubes[type], transform, sunLight, otherLight));
}

void
CubeBatcher::draw() {

  m_program.bind();

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glActiveTexture(GL_TEXTURE0);
  m_program.setUniformli(arrayTexture, 0);
  m_texture.bind();

  m_program.setUniform1f(sunStrength, m_sunStrength);

  for (Batch b : m_batches) {
    m_program.setUniform1f(sunLight, b.m_sunLight);
    m_program.setUniform1f(otherLight, b.m_otherLight);

    glm::mat4 modelView = m_camera.getViewMatrix() * b.m_transform.getMatrix();
    glm::mat4 modelViewProjection = m_camera.getProjectionMatrix() * modelView;
    m_program.setUniformMatrix4f(mvp, modelViewProjection);
    b.m_cube.draw();
  }

  m_program.unbind();
  m_batches.clear();
}

void
CubeBatcher::setSunStrenght(float value) {
  m_sunStrength = value;
}

} /* namespace graphics */
