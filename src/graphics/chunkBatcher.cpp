
#include "chunkBatcher.h"

#include <map>

#include "shaderProgram.h"
#include "texture/textureArray.h"

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
	// TODO
}

void ChunkBatcher::draw(Camera &camera)
{

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
	static TextureArray texture{cube_data::textures, cube_data::TEXTURE_WIDTH, cube_data::TEXTURE_HEIGHT};

	program.bind();

	glActiveTexture(GL_TEXTURE0);
	program.setUniformli("texture1", 0);
	texture.bind();

	for (auto b : batches) {
		glm::mat4 modelView = camera.getViewMatrix() * b->getTransform().getMatrix();
		glm::mat4 modelViewProjection = camera.getProjectionMatrix() * modelView;
		program.setUniformMatrix4f("modelViewProjection", modelViewProjection);
		b->draw();
	}

}
