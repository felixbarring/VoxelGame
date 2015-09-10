
#include "chunkBatcher.h"

#include <map>

#include "resources.h"
#include "shaderProgram.h"
#include "texture/textureArray.h"

namespace graphics
{


// ########################################################
// Constructor/Destructor #################################
// ########################################################

ChunkBatcher::ChunkBatcher()
{
}

ChunkBatcher::~ChunkBatcher()
{
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
	// Should be some where else!

	static const char *vertex =
		"#version 330 core \n"

		"in vec3 positionIn; \n"
		"in vec3 normalIn; \n"
		"in vec3 texCoordIn; \n"

		"uniform mat4 modelViewProjection; \n"

		"out vec3 faceNormal; \n"
		"out vec3 texCoord; \n"

		"void main(){ \n"
		"  texCoord = vec3(texCoordIn.x, 1 - texCoordIn.y, texCoordIn.z); \n"
		"  gl_Position =  modelViewProjection * vec4(positionIn, 1); \n"
		"} \n";

	static const char *fragment =
		"#version 330 core \n"

		"in vec3 texCoord; \n"

		"uniform sampler2DArray texture1; \n"

		"out vec4 color; \n"

		"void main(){ \n"
		"  color = texture(texture1, texCoord); \n"
		"} \n";

	static std::map<std::string, int> attributesMap{
		std::pair<std::string, int>("positionIn", 0),
		std::pair<std::string, int>("normalIn", 1),
		std::pair<std::string, int>("texCoordIn", 2)
	};

	static ShaderProgram program(vertex, fragment, attributesMap);
	static texture::TextureArray &texture = Resources::getInstance().getTextureArray(config::cube_data::textures, config::cube_data::TEXTURE_WIDTH, config::cube_data::TEXTURE_HEIGHT);

	program.bind();

	glActiveTexture(GL_TEXTURE0);
	program.setUniformli("texture1", 0);
	texture.bind();

	Camera &camera = Camera::getInstance();

	for (auto b : batches) {
		glm::mat4 modelView = camera.getViewMatrix() * b->getTransform().getMatrix();
		glm::mat4 modelViewProjection = camera.getProjectionMatrix() * modelView;
		program.setUniformMatrix4f("modelViewProjection", modelViewProjection);
		b->draw();
	}

	program.unbind();

}


}

