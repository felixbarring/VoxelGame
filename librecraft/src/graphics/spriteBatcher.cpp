#include "spriteBatcher.h"

#include <iostream>
#include <map>
#include <algorithm>

#include "shaderProgram.h"

using namespace std;

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

SpriteBatcher::SpriteBatcher() {
	// hard coded default value
	m_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	const char *vertex =
		"#version 330 core \n"
		"in vec3 positionIn; \n"
		"in vec2 texCoordIn; \n"

		"uniform mat4 projection; \n"

		"out vec2 texCoord; \n"

		"void main() \n"
		"{ \n"
		"  gl_Position = projection * vec4(positionIn, 1.0f); \n"
		"  texCoord = texCoordIn; \n"
		"} \n";

	const char *frag =
		"#version 330 core \n"
		"in vec2 texCoord; \n"

		"out vec4 color; \n"

		"uniform sampler2D texture1; \n"
		"void main() \n"
		"{ \n"
		"  color = texture(texture1, texCoord); \n"
		"} \n";


	map<string, int> attributesMap{
		pair<string, int>("positionIn", 0),
		pair<string, int>("texCoordIn", 1)
	};

	m_program.reset(new ShaderProgram{vertex, frag, attributesMap});

}

// ########################################################
// Member Functions########################################
// ########################################################
void SpriteBatcher::addBatch(shared_ptr<Sprite> batch) {
	m_batches.push_back(batch);
}

void SpriteBatcher::draw() {

	m_program->bind();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	sort(m_batches.begin(), m_batches.end(),
			[](shared_ptr<Sprite> a, shared_ptr<Sprite> b) -> bool
			{
				return a->getLayer() < b->getLayer();
			});

	m_program->setUniformli("texture1", 0);

	texture::Texture *current;

	for (auto batch : m_batches) {
		glActiveTexture(GL_TEXTURE0);
		if (&batch->getTexture() != current) {
			current = &batch->getTexture();
			current->bind();
		}

		glm::mat4 modelViewProjection { m_projection
				* batch->getTransform().getMatrix() };

		m_program->setUniformMatrix4f("projection", modelViewProjection);
		batch->draw();
	}

	m_program->unbind();
	m_batches.clear();
}

void SpriteBatcher::setProjection(glm::mat4 projection) {
	this->m_projection = projection;
}


} /* namespace graphics */