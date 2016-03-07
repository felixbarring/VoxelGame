#include "chunkBatcher.h"

#include <map>
#include <iostream>

#include "resources.h"
#include "shaderProgram.h"

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ChunkBatcher::ChunkBatcher()
		: texture(
				Resources::getInstance().getTextureArray(
						config::cube_data::textures,
						config::cube_data::TEXTURE_WIDTH,
						config::cube_data::TEXTURE_HEIGHT)) {
	const char *vertex =
			"#version 330 core \n"

			"const float density = 0.001; \n"
			"const float gradient = 6; \n"

			"in vec4 positionIn; \n"
			"in vec3 normalIn; \n"
			"in vec3 texCoordIn; \n"

			"uniform mat4 modelViewProjection; \n"
			"uniform mat4 modelView; \n"

			"out vec3 faceNormal; \n"
			"out vec3 texCoord; \n"
			"out float lightValue; \n"
			"out float fogFactor; \n "

			"void main(){ \n"
			"  texCoord = vec3(texCoordIn.x, texCoordIn.y, texCoordIn.z); \n"
			"  lightValue = positionIn.w / 16; \n"

			"  vec4 positionView = modelView * vec4(positionIn.xyz, 1); \n"
			"  float distance = length(positionView.xyz); \n"
			"  fogFactor = clamp(exp(-pow((distance * density), gradient)), 0.0, 1.0); \n"

			"  gl_Position =  modelViewProjection * vec4(positionIn.xyz, 1); \n"
			"} \n";

	const char *fragment =
			"#version 330 core \n"

			"const vec3 fogColor = vec3(0.2, 0.22, 0.2); \n"

			"in vec3 texCoord; \n"
			"in float lightValue; \n"
			"in float fogFactor; \n"

			"uniform sampler2DArray texture1; \n"

			"out vec4 color; \n"

			"void main(){ \n"

			"  color = mix(vec4(fogColor, 1.0), "
			"    vec4(lightValue, lightValue, lightValue, 1) * texture(texture1, texCoord), "
			"    fogFactor);"

			"} \n";

	std::map<std::string, int> attributesMap {std::pair<std::string, int>(
			"positionIn", 0), std::pair<std::string, int>("normalIn", 1),
			std::pair<std::string, int>("texCoordIn", 2)};

	program.reset(new ShaderProgram(vertex, fragment, attributesMap));

}

// ########################################################
// Member Functions########################################
// ########################################################

void ChunkBatcher::addBatch(std::shared_ptr<GraphicalChunk> batch) {
	batches.push_back(batch);
}

void ChunkBatcher::removeBatch(std::shared_ptr<GraphicalChunk> batch) {

	for (unsigned i = 0; i < batches.size(); ++i) {
		if (batches.at(i).get() == batch.get()) {
			batches.erase(batches.begin() + i);
			return;
		}
	}
}

void ChunkBatcher::draw() {
	program->bind();

	glActiveTexture(GL_TEXTURE0);
	program->setUniformli("texture1", 0);
	texture.bind();

	Camera &camera = Camera::getInstance();

	float totalTime = 0;

	for (auto b : batches) {

		// TODO Do frustrum culling here

//		auto t = glfwGetTime();

		glm::mat4 modelView = camera.getViewMatrix()
				* b->getTransform().getMatrix();
		glm::mat4 modelViewProjection = camera.getProjectionMatrix()
				* modelView;

//		totalTime += (glfwGetTime() - t);

		program->setUniformMatrix4f("modelViewProjection", modelViewProjection);
		program->setUniformMatrix4f("modelView", modelView);
		b->draw();
	}

//	std::cout << "Total matrix time: " << totalTime << "\n";

	program->unbind();

}

}

