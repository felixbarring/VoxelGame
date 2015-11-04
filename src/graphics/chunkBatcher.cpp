
#include "chunkBatcher.h"

#include <map>

#include "resources.h"
#include "shaderProgram.h"

namespace graphics
{


// ########################################################
// Constructor/Destructor #################################
// ########################################################

ChunkBatcher::ChunkBatcher() :
	texture(Resources::getInstance().getTextureArray(
			config::cube_data::textures,
			config::cube_data::TEXTURE_WIDTH,
			config::cube_data::TEXTURE_HEIGHT))
{
	const char *vertex =
		"#version 330 core \n"

		"in vec4 positionIn; \n"
		"in vec3 normalIn; \n"
		"in vec3 texCoordIn; \n"

		"uniform mat4 modelViewProjection; \n"

		"out vec3 faceNormal; \n"
		"out vec3 texCoord; \n"
		"out float lightValue; \n"

		"void main(){ \n"
		//"  texCoord = vec3(texCoordIn.x, 1 - texCoordIn.y, texCoordIn.z); \n"
		"  texCoord = vec3(texCoordIn.x, texCoordIn.y, texCoordIn.z); \n"
		//"  lightValue = (positionIn.w + 3) / 16; \n"
		"  lightValue = positionIn.w / 16; \n"

		"  gl_Position =  modelViewProjection * vec4(positionIn.xyz, 1); \n"
		"} \n";

	const char *fragment =
		"#version 330 core \n"

		"in vec3 texCoord; \n"
		"in float lightValue; \n"

		"uniform sampler2DArray texture1; \n"

		"out vec4 color; \n"

		"void main(){ \n"
		"  color = vec4(lightValue, lightValue, lightValue, 1) * texture(texture1, texCoord); \n"
		"} \n";

	std::map<std::string, int> attributesMap{
		std::pair<std::string, int>("positionIn", 0),
		std::pair<std::string, int>("normalIn", 1),
		std::pair<std::string, int>("texCoordIn", 2)
	};

	program.reset(new ShaderProgram(vertex, fragment, attributesMap));

}

// ########################################################
// Member Functions########################################
// ########################################################

void ChunkBatcher::addBatch(std::shared_ptr<GraphicalChunk> batch)
{
	batches.push_back(batch);
}

void ChunkBatcher::removeBatch(std::shared_ptr<GraphicalChunk> batch)
{

	for (unsigned i = 0; i < batches.size(); ++i) {
		if (batches.at(i).get() == batch.get()) {
			batches.erase(batches.begin() + i);
			return;
		}
	}
}

void ChunkBatcher::draw()
{
	program->bind();

	glActiveTexture(GL_TEXTURE0);
	program->setUniformli("texture1", 0);
	texture.bind();

	Camera &camera = Camera::getInstance();

	for (auto b : batches) {

		// TODO Matrix multiplications are a bottle neck?
		// TODO Consider using an add instead of multiplication.
		glm::mat4 modelViewProjection = camera.getProjectionMatrix() * (camera.getViewMatrix() * b->getTransform().getMatrix());
		program->setUniformMatrix4f("modelViewProjection", modelViewProjection);
		b->draw();
	}
	program->unbind();

}


}

