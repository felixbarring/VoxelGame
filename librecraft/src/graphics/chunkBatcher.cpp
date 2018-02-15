#include "chunkBatcher.h"

#include <algorithm>
#include <iostream>
#include <map>

#include "frustum.h"
#include "resources.h"
#include "shaderProgram.h"

using namespace std;
using namespace glm;

namespace graphics {

ShaderProgram createShader() {
  // TODO Refactor this the same way as the cubeBatcher...
  // That is, use constant strings for the variables.

  // clang-format off
  string vertex =
    "#version 330 core \n"

    "in vec3 positionIn; \n"
    "in vec2 lightIn; \n"
    "in vec3 normalIn; \n"
    "in vec3 texCoordIn; \n"

    "uniform mat4 modelViewProjection; \n"
    "uniform mat4 modelView; \n"
    "uniform float sunStrenght; \n"

    "out vec3 texCoord; \n"
    "out float lightValue; \n"
    "out vec3 faceNormal; \n"

    "void main(){ \n"
    "  texCoord = vec3(texCoordIn.x, texCoordIn.y, texCoordIn.z); \n"
    "  lightValue = max(lightIn.x * sunStrenght, lightIn.y) / 16; \n"
    "  faceNormal = normalIn; \n"
    "  gl_Position =  modelViewProjection * vec4(positionIn.xyz, 1); \n"
    "} \n";

  string fragment =
    "#version 330 core \n"

    "in vec3 texCoord; \n"
    "in float lightValue; \n"
    "in vec3 faceNormal; \n"

    "uniform sampler2DArray texture1; \n"
    "uniform float sunStrenght; \n"

    "uniform vec3 lightDirection; \n"
    "uniform vec3 diffuseLight = vec3(0.5, 0.5, 0.5); \n"
    "uniform vec3 materialDiffuse = vec3(0.5, 0.5, 0.5); \n"

    "out vec4 color; \n"

    "vec3 calculateDiffuse() \n "
    "{ \n "
    "  return sunStrenght * diffuseLight * materialDiffuse * "
    "    max(0, dot(faceNormal, normalize(lightDirection))); \n "
    "} \n "

    "void main(){ \n"
    "  vec3 lightSum = vec3(lightValue, lightValue, lightValue) + "
    "(calculateDiffuse() / 2); \n"
    "  color = vec4(lightSum, 1.0f) * texture(texture1, texCoord); \n"
    "} \n";

  // clang-format on

  map<string, int> attributesMap{pair<string, int>("positionIn", 0),
                                 pair<string, int>("lightIn", 1),
                                 pair<string, int>("normalIn", 2),
                                 pair<string, int>("texCoordIn", 3)};

  return ShaderProgram(vertex, fragment, attributesMap);
}

ChunkBatcher::ChunkBatcher(Camera& camera)
  : m_camera(camera)
  , m_program{createShader()}
  , m_texture(Resources::getInstance().getTextureArray(
      config::cube_data::textures,
      config::cube_data::TEXTURE_WIDTH,
      config::cube_data::TEXTURE_HEIGHT))
{
}

// ########################################################
// Member Functions########################################
// ########################################################

int
ChunkBatcher::addBatch(int replaceId, GraphicalChunk&& batch) {
  lock_guard<mutex> lock(m_mutex);
  m_batchesToAdd.push_back(make_tuple(++m_idCounter, replaceId, move(batch)));
  return m_idCounter;
}

void
ChunkBatcher::removeBatch(int id) {
  lock_guard<mutex> lock(m_mutex);
  m_batchesToBeRemoved.push_back(id);
}

float x = 1.0;
int direction = 1;

void
ChunkBatcher::draw() {
  // Done on the main thread because the thread doing opengl
  // calls needs an opengl context, which the main thread does.
  {
    lock_guard<mutex> lock(m_mutex);
    addAndRemoveBatches();
  }

  m_program.bind();

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glActiveTexture(GL_TEXTURE0);
  m_program.setUniformli("texture1", 0);
  m_texture.bind();

  m_program.setUniform3f("lightDirection", x, 3.0, 0.3);
  m_program.setUniform1f("sunStrenght", m_sunStrength);

  //    m_program->setUniform3f("fogColor", skyColor.x, skyColor.y, skyColor.z);

  for (pair<const int, GraphicalChunk>& batch : m_batches) {
    mat4 modelView =
      m_camera.getViewMatrix() * batch.second.getTransform().getMatrix();
    mat4 modelViewProjection = m_camera.getProjectionMatrix() * modelView;

    m_program.setUniformMatrix4f("modelViewProjection", modelViewProjection);
    m_program.setUniformMatrix4f("modelView", modelView);

    batch.second.drawNoneTransparent();
  }

  glDisable(GL_CULL_FACE);

  // TODO Reuse matrixes
  // A second pass to draw the water/transparent stuffs
  for (pair<const int, GraphicalChunk>& batch : m_batches) {
    if (!batch.second.hasTransparent())
      continue;

    mat4 modelView =
      m_camera.getViewMatrix() * batch.second.getTransform().getMatrix();
    mat4 modelViewProjection = m_camera.getProjectionMatrix() * modelView;
    m_program.setUniformMatrix4f("modelViewProjection", modelViewProjection);
    m_program.setUniformMatrix4f("modelView", modelView);
    batch.second.drawTransparent();
  }

  m_program.unbind();
}

void
ChunkBatcher::setSunStrenght(double value) {
  m_sunStrength = value;
}

void
ChunkBatcher::addAndRemoveBatches() {
  // Add one of the batches with none high priority that has been requested to
  // be added.
  for (tuple<int, int, GraphicalChunk>& t : m_batchesToAdd) {
    int id{get<0>(t)};
    int replaceId{get<1>(t)};

    m_batches.emplace(id, move(get<2>(t)));
    m_batches.at(id).uploadData();

    if (replaceId != m_noRemove)
      m_batchesToBeRemoved.push_back(replaceId);
  }
  m_batchesToAdd.clear();

  // Remove all of the batches that has been requested to be removed.
  while (!m_batchesToBeRemoved.empty()) {
    vector<int>::iterator batch = m_batchesToBeRemoved.begin();
    map<int, GraphicalChunk>::iterator batchIt = m_batches.find(*batch);

    if (batchIt != m_batches.end()) {
      m_batches.erase(batchIt);
      m_batchesToBeRemoved.erase(batch);
    }
  }
}
}

// string vertex =
//    "#version 330 core \n"
//
//    "const float density = 0.01; \n"
//    "const float gradient = 6; \n"
//
//    "in vec4 positionIn; \n"
//    "in vec3 normalIn; \n"
//    "in vec3 texCoordIn; \n"
//
//    "uniform mat4 modelViewProjection; \n"
//    "uniform mat4 modelView; \n"
//    "uniform float sunStrenght; \n"
//
//    "out vec3 texCoord; \n"
//    "out float lightValue; \n"
//    "out float fogFactor; \n"
//    "out vec3 faceNormal; \n"
//
//    "void main(){ \n"
//    "  texCoord = vec3(texCoordIn.x, texCoordIn.y, texCoordIn.z); \n"
//    "  lightValue = (positionIn.w / 16) * sunStrenght; \n"
//    "  faceNormal = normalIn; \n"
//
//    "  vec4 positionView = modelView * vec4(positionIn.xyz, 1); \n"
//    "  float distance = length(positionView.xyz); \n"
//    "  fogFactor = clamp(exp(-pow((distance * density), gradient)), 0.0, 1.0);
//    \n"
//
//    "  gl_Position =  modelViewProjection * vec4(positionIn.xyz, 1); \n"
//    "} \n";
//
// string fragment =
//    "#version 330 core \n"
//
//    "in vec3 texCoord; \n"
//    "in float lightValue; \n"
//    "in float fogFactor; \n"
//
//    "in vec3 faceNormal; \n"
//
//    "uniform sampler2DArray texture1; \n"
//    "uniform float sunStrenght; \n"
//
//    "out vec4 color; \n"
//
//    "uniform vec3 lightDirection; \n"
//    "uniform vec3 fogColor; \n"
//
//    "uniform vec3 diffuseLight = vec3(0.5, 0.5, 0.5); \n"
//    "uniform vec3 materialDiffuse = vec3(0.5, 0.5, 0.5); \n"
//
//    "vec3 calculateDiffuse() \n "
//    "{ \n "
//    "  return sunStrenght * diffuseLight * materialDiffuse * max(0,
//    dot(faceNormal, normalize(lightDirection))); \n "
//    "} \n "
//
//    "void main(){ \n"
//
//    "  vec3 diffuse = calculateDiffuse() / 2; \n"
//    "  color = vec4(diffuse, 1.0f) * texture(texture1, texCoord); \n"
//
//    "  vec4 light = vec4(lightValue, lightValue, lightValue, 1) +
//    vec4(diffuse, 0); \n"
//    "  color = mix(vec4(fogColor, 1.0), light * texture(texture1, texCoord),
//    fogFactor);"
//
//    "} \n";
