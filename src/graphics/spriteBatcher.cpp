/*
 * spriteBatcher.cpp
 *
 *  Created on: Aug 26, 2015
 *      Author: felix
 */

#include "spriteBatcher.h"

#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderProgram.h"

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

SpriteBatcher::SpriteBatcher()
{
}

SpriteBatcher::~SpriteBatcher()
{
}



// ########################################################
// Member Functions########################################
// ########################################################
void SpriteBatcher::addBatch(std::shared_ptr<Sprite> batch)
{
	batches.push_back(batch);
}

void SpriteBatcher::draw()
{

	static const char *vertex =
		"#version 330 core \n"
		"in vec3 positionIn; \n"
		"in vec2 texCoordIn; \n"

		"uniform mat4 projection; \n"

		"out vec2 texCoord; \n"

		"void main() \n"
		"{ \n"
		"  gl_Position = projection * vec4(positionIn, 1.0f); \n"
		"  texCoord = vec2(texCoordIn.x, 1.0 - texCoordIn.y); \n"
		"} \n";

	static const char *frag =
		"#version 330 core \n"
		"in vec2 texCoord; \n"

		"out vec4 color; \n"

		"uniform sampler2D texture1; \n"
		"void main() \n"
		"{ \n"
		"  color = texture(texture1, texCoord); \n"
		"} \n";


	static std::map<std::string, int> attributesMap{
		std::pair<std::string, int>("positionIn", 0),
		std::pair<std::string, int>("texCoordIn", 1)
	};

	static graphics::ShaderProgram program(vertex, frag, attributesMap);
	static glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	program.bind();

	for (std::shared_ptr<Sprite> batch : batches) {
		glActiveTexture(GL_TEXTURE0);

		batch->getTexture().bind();
		program.setUniformli("texture1", 0);

		//sprite.getTransform().rotateZ(0.1);

		//batch->getTransform().rotateZ(0.1);

		glm::mat4 modelViewProjection = projection * batch->getTransform().getMatrix();

		program.setUniformMatrix4f("projection", modelViewProjection);

		batch->draw();
	}
}


} /* namespace graphics */
