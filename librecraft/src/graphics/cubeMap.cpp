#include "cubeMap.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/camera.h"
#include "../graphics/texture/textureCubeMap.h"
#include "../graphics/mesh/meshElement.h"

namespace graphics {

CubeMap::CubeMap(texture::TextureCubeMap &texture, Camera &camera)
  : m_texture{texture}
  , m_camera{camera}
{
}

void CubeMap::setRotationValue(float value) {
  m_roatationValue = value;
}

void CubeMap::draw(double trancparency) {

  std::string vertex =
    "#version 330 core \n"
    "in vec3 positionIn; \n"
    "uniform mat4 mvp; \n"
    "out vec3 texCoord; \n"
    "void main() \n"
    "{ \n"
    "  vec4 pos = mvp * vec4(positionIn, 1.0); \n"
    "  gl_Position = pos.xyww; \n"
    "  texCoord = positionIn; \n"
    "} \n";

  std::string fragment =
    "#version 330 core \n"
    "in vec3 texCoord; \n"
    "uniform samplerCube skybox; \n"
    "uniform float transparency; \n"
    "out vec4 color; \n"
    "void main() \n"
    "{ \n"
    "  vec4 tempColor = texture(skybox, texCoord); \n"
//    "  tempColor.w = tempColor.w * transparency; \n"
    "  color = tempColor; \n"
    "} \n";

  static std::map<std::string, int> attributesMap{{"positionIn", 0}};

  static ShaderProgram skyboxShader(vertex, fragment, attributesMap);

  static std::vector<GLfloat> vert{
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
  };

  static std::vector<short> element {
    0, 1, 2, 3, 4, 5,
    0+6, 1+6, 2+6, 3+6, 4+6, 5+6,
    0+12, 1+12, 2+12, 3+12, 4+12, 5+12,

    0+18, 1+18, 2+18, 3+18, 4+18, 5+18,
    0+24, 1+24, 2+24, 3+24, 4+24, 5+24,
    0+30, 1+30, 2+30, 3+30, 4+30, 5+30,
  };

  static mesh::MeshElement mesh{vert, 3, element};

  skyboxShader.bind();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_roatationValue,
      glm::vec3(0.1, 0.3, 1));
  glm::mat4 view = glm::mat4(glm::mat3(m_camera.getViewMatrix())) * rotation;

  glm::mat4 modelViewProjection = m_camera.getProjectionMatrix() * view;

  skyboxShader.setUniformMatrix4f("mvp", modelViewProjection);
  skyboxShader.setUniform1f("transparency", trancparency);
  m_texture.bind();
  mesh.draw();

  skyboxShader.unbind();
}

}
