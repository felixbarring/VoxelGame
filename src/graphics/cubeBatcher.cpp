
#include "cubeBatcher.h"

#include <string>
#include <map>
#include <iostream>

#include "shaderProgram.h"
#include "texture/textureArray.h"
#include "../config/data.h"
#include "resources.h"

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

CubeBatcher::CubeBatcher()
{
	for (int i = 0; i <= config::cube_data::LAST_BLOCK + 1; i++) {
		cubes.push_back(TexturedCube{2, 0, -1.0f, i});
	}
}

// ########################################################
// Member Functions########################################
// ########################################################

void CubeBatcher::addBatch(char type, Transform &transform)
{
	batches.push_back(Batch(cubes.at(type-1), transform));
}

void CubeBatcher::draw()
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


	// Use Smart Pointer
	static std::map<std::string, int> attributesMap{
		std::pair<std::string, int>("positionIn", 0),
		std::pair<std::string, int>("normalIn", 1),
		std::pair<std::string, int>("texCoordIn", 2)
	};

	static graphics::ShaderProgram program(vertex, fragment, attributesMap);
	static texture::TextureArray &texture = graphics::Resources::getInstance().getTextureArray(
			config::cube_data::textures, config::cube_data::TEXTURE_WIDTH, config::cube_data::TEXTURE_HEIGHT);

	program.bind();

	glActiveTexture(GL_TEXTURE0);
	program.setUniformli("texture1", 0);
	texture.bind();

	Camera& camera = Camera::getInstance();

	for (auto b : batches) {
		glm::mat4 modelView = camera.getViewMatrix() * b.transform.getMatrix();
		glm::mat4 modelViewProjection = camera.getProjectionMatrix() * modelView;
		program.setUniformMatrix4f("modelViewProjection", modelViewProjection);
		b.cube.draw();
	}

	program.unbind();

	batches.clear();
}


} /* namespace graphics */
