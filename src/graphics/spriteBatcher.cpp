
#include "spriteBatcher.h"

#include <map>
#include <algorithm>

#include "shaderProgram.h"

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

SpriteBatcher::SpriteBatcher()
{
	// hard coded default value
	projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
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
		"  texCoord = texCoordIn; \n"
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

	program.bind();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	std::sort(batches.begin(), batches.end(),
		[](std::shared_ptr<Sprite> a, std::shared_ptr<Sprite> b) -> bool
	    {
	      return a->layer < b->layer;
	    }
	);

	for (std::shared_ptr<Sprite> batch : batches) {

		glActiveTexture(GL_TEXTURE0);
		batch->getTexture().bind();
		program.setUniformli("texture1", 0);

		glm::mat4 modelViewProjection = projection * batch->getTransform().getMatrix();

		program.setUniformMatrix4f("projection", modelViewProjection);
		batch->draw();
	}

	program.unbind();

	batches.clear();
}

void SpriteBatcher::setProjection(glm::mat4 projection)
{
	this->projection = projection;
}


} /* namespace graphics */
