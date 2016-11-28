#include "chunkBatcher.h"

#include <map>
#include <iostream>

#include "frustum.h"
#include "resources.h"
#include "shaderProgram.h"

using namespace std;
using namespace glm;

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ChunkBatcher::ChunkBatcher(Camera &camera)
    : m_texture(Resources::getInstance().getTextureArray(
                    config::cube_data::textures,
                    config::cube_data::TEXTURE_WIDTH,
                    config::cube_data::TEXTURE_HEIGHT)),
      m_camera(camera)
{

    const char *vertex =
            "#version 330 core \n"

            "const float density = 0.01; \n"
            "const float gradient = 6; \n"

            "in vec4 positionIn; \n"
            "in vec3 normalIn; \n"
            "in vec3 texCoordIn; \n"

            "uniform mat4 modelViewProjection; \n"
            "uniform mat4 modelView; \n"
            "uniform float sunStrenght; \n"

            "out vec3 texCoord; \n"
            "out float lightValue; \n"
            "out float fogFactor; \n"
            "out vec3 faceNormal; \n"

            "void main(){ \n"
            "  texCoord = vec3(texCoordIn.x, texCoordIn.y, texCoordIn.z); \n"
            "  lightValue = (positionIn.w / 16) * sunStrenght; \n"
            "  faceNormal = normalIn; \n"

            "  vec4 positionView = modelView * vec4(positionIn.xyz, 1); \n"
            "  float distance = length(positionView.xyz); \n"
            "  fogFactor = clamp(exp(-pow((distance * density), gradient)), 0.0, 1.0); \n"

            "  gl_Position =  modelViewProjection * vec4(positionIn.xyz, 1); \n"
            "} \n";

    const char *fragment =
            "#version 330 core \n"

            "in vec3 texCoord; \n"
            "in float lightValue; \n"
            "in float fogFactor; \n"

            "in vec3 faceNormal; \n"

            "uniform sampler2DArray texture1; \n"
            "uniform float sunStrenght; \n"

            "out vec4 color; \n"

            "uniform vec3 lightDirection; \n"
            "uniform vec3 fogColor; \n"

            "uniform vec3 diffuseLight = vec3(0.5, 0.5, 0.5); \n"
            "uniform vec3 materialDiffuse = vec3(0.5, 0.5, 0.5); \n"

            "vec3 calculateDiffuse() \n "
            "{ \n "
            "  return sunStrenght * diffuseLight * materialDiffuse * max(0, dot(faceNormal, normalize(lightDirection))); \n "
            "} \n "

            "void main(){ \n"

            "  vec3 diffuse = calculateDiffuse() / 2; \n"
            "  color = vec4(diffuse, 1.0f) * texture(texture1, texCoord); \n"

            "  vec4 light = vec4(lightValue, lightValue, lightValue, 1) + vec4(diffuse, 0); \n"
            "  color = mix(vec4(fogColor, 1.0), light * texture(texture1, texCoord), fogFactor);"

            "} \n";

    map<string, int> attributesMap{
        pair<string, int>("positionIn", 0),
        pair<string, int>("normalIn", 1),
        pair<string, int>("texCoordIn", 2)};

    m_program.reset(new ShaderProgram(vertex, fragment, attributesMap));

}

// ########################################################
// Member Functions########################################
// ########################################################

int ChunkBatcher::addBatch(
        int replaceId,
        float x, float y, float z,
        VoxelMatrix &data,
        VoxelMatrix *right,
        VoxelMatrix *left,
        VoxelMatrix *back,
        VoxelMatrix *front, bool hightPriority) {

    auto batch = make_shared<GraphicalChunk>(x, y, z, data, right, left, back, front);

    lock_guard<mutex> lock(m_mutex);
    if (hightPriority)
        m_batchesToAddHP.push_back(make_tuple(++m_idCounter, replaceId, batch));
    else
        m_batchesToAdd.push_back(make_tuple(++m_idCounter, replaceId, batch));

    return m_idCounter;
}

void ChunkBatcher::removeBatch(int id) {
    lock_guard<mutex> lock(m_mutex);
    m_batchesToBeRemoved.push_back(id);
}

float x = 1.0;
int direction = 1;

void ChunkBatcher::draw() {

    // Done on the main thread because the thread doing opengl
    // calls needs an opengl context, which the main thread does.
    lock_guard<mutex> lock(m_mutex);

    // Add all the batches that has high priority
    while (!m_batchesToAddHP.empty()) {
        auto batchIt = m_batchesToAddHP.begin();
        get<2>(*batchIt)->uploadData();
        m_batches.emplace(get<0>(*batchIt), get<2>(*batchIt));
        m_batchesToAddHP.erase(batchIt);

        auto removeId{get<1>(*batchIt)};
        if (removeId != noRemove) {
            auto batchIt = m_batches.find(removeId);
            if (batchIt != m_batches.end())
                m_batches.erase(batchIt);
        }
    }

    // Add one of the batches that has been requested to be added.
    if (!m_batchesToAdd.empty()) {
        auto batchIt = m_batchesToAdd.begin();
        get<2>(*batchIt)->uploadData();
        m_batches.emplace(get<0>(*batchIt), get<2>(*batchIt));
        m_batchesToAdd.erase(batchIt);

        auto removeId{get<1>(*batchIt)};
        if (removeId != noRemove) {
            m_batchesToBeRemoved.push_back(removeId);
        }
    }

    // Remove all of the batches that has been requested to be removed.
    while (!m_batchesToBeRemoved.empty()) {
        auto batch = m_batchesToBeRemoved.begin();
        auto batchIt = m_batches.find(*batch);
        if (batchIt != m_batches.end())
            m_batches.erase(batchIt);
        else {

            // TODO Solve the problem with gchunks not getting removed correctly.

            cout << "GChunk can not be removed. ID = " << *batch << "\n";
            for (auto b = m_batchesToAdd.begin(); b != m_batchesToAdd.end(); ++b) {
                std::cout << "   " << get<0>(*b) << "\n";
                if (get<0>(*b) == *batch) {
                    m_batchesToAdd.erase(b);
                    return;
                }
            }
            cout << "Failed :( \n";

        }

        m_batchesToBeRemoved.clear();
    }

    m_program->bind();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    m_program->setUniformli("texture1", 0);
    m_texture.bind();

    //	if (x > 5 || x < - 5)
    //		direction = -direction;
    //	x += 0.1 * direction;

    m_program->setUniform3f("lightDirection", x, 3.0, 0.3);
    m_program->setUniform1f("sunStrenght", m_sunStrength);

    vec3 skyColor = config::graphics_data::skyColor;
    vec3 dark{0,0,0};
    skyColor = mix(dark, skyColor, m_sunStrength);
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
    m_program->setUniform3f("fogColor", skyColor.x, skyColor.y, skyColor.z);

    int skippedChunks = 0;

    for (auto batch : m_batches) {
        mat4 modelView = m_camera.getViewMatrix() * batch.second->getTransform().getMatrix();
        mat4 modelViewProjection = m_camera.getProjectionMatrix() * modelView;

        // TODO Fix this so that no chunks get culled when they are actually vissible
    //		Frustum frustum{modelViewProjection};
    //		bool result = frustum.isCubeInFrustum(batch.second->getxLocation(), 0, batch.second->getzLocation(), 16, 128, 16);
    ////		bool result = frustum.isSphereInFrustum(vec3(batch.second->getxLocation() + 8, 64, batch.second->getzLocation() + 8), 100);
    //
    //		if (!result) {
    //			++skippedChunks;
    //			continue;
    //		}

        m_program->setUniformMatrix4f("modelViewProjection", modelViewProjection);
        m_program->setUniformMatrix4f("modelView", modelView);

        batch.second->drawNoneTransparent();
    }

    glDisable(GL_CULL_FACE);

    // TODO Reuse matrixes
    // A second pass to draw the water/transparent stuffs
    for (auto batch : m_batches) {
        if (!batch.second->hasTransparent())
            continue;

        mat4 modelView = m_camera.getViewMatrix() * batch.second->getTransform().getMatrix();
        mat4 modelViewProjection = m_camera.getProjectionMatrix() * modelView;
        m_program->setUniformMatrix4f("modelViewProjection", modelViewProjection);
        m_program->setUniformMatrix4f("modelView", modelView);
        batch.second->drawTransparent();
    }

    m_program->unbind();
}

void ChunkBatcher::setSunStrenght(float value) {
    m_sunStrength = value;
}

}
