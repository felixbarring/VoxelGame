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

CubeBatcher::CubeBatcher()
	: texture(
			graphics::Resources::getInstance().getTextureArray(
					config::cube_data::textures,
					config::cube_data::TEXTURE_WIDTH,
					config::cube_data::TEXTURE_HEIGHT))
{

	for (int i = 0; i <= config::cube_data::LAST_CUBE + 1; i++) {
		cubes.push_back(TexturedCube {2, 0, -1.0f, i});
	}


	const char *vertex =
		"#version 330 core \n"

		"in vec3 positionIn; \n"
		"in vec3 normalIn; \n"
		"in vec3 texCoordIn; \n"

		"uniform mat4 modelViewProjection; \n"

		"out vec3 faceNormal; \n"
		"out vec3 texCoord; \n"

		"void main(){ \n"
		"  texCoord = vec3(texCoordIn.x, texCoordIn.y, texCoordIn.z); \n"
		"  gl_Position =  modelViewProjection * vec4(positionIn, 1); \n"
		"} \n";

	const char *fragment =
		"#version 330 core \n"

		"in vec3 texCoord; \n"

		"uniform sampler2DArray texture1; \n"
		"uniform float lightValue; \n"

		"out vec4 color; \n"

		"void main(){ \n"
		"  color = (lightValue / 16) * texture(texture1, texCoord); \n"
		"  color.w = 1.0; \n"
		"} \n";

	std::map<std::string, int> attributesMap{
		std::pair<std::string, int>("positionIn", 0),
		std::pair<std::string, int>("normalIn", 1),
		std::pair<std::string, int>("texCoordIn", 2)
	};

	program.reset(new ShaderProgram{vertex, fragment, attributesMap});

}

// ########################################################
// Member Functions########################################
// ########################################################

void CubeBatcher::addBatch(char type, Transform &transform, int lightValue) {
	batches.push_back(Batch(cubes.at(type), transform, lightValue));
}

void CubeBatcher::draw() {

	program->bind();

	glEnable (GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	program->setUniformli("texture1", 0);
	texture.bind();

	Camera& camera = Camera::getInstance();

	for (auto b : batches) {
		program->setUniform1f("lightValue", b.m_lightValue);

		glm::mat4 modelView = camera.getViewMatrix() * b.m_transform.getMatrix();
		glm::mat4 modelViewProjection = camera.getProjectionMatrix() * modelView;
		program->setUniformMatrix4f("modelViewProjection", modelViewProjection);
		b.m_cube.draw();
	}

	program->unbind();
	batches.clear();
}

} /* namespace graphics */
