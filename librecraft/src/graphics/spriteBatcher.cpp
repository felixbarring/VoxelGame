
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

SpriteBatcher::SpriteBatcher()
{
	// hard coded default value
	projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

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

	program.reset(new ShaderProgram{vertex, frag, attributesMap});

}

// ########################################################
// Member Functions########################################
// ########################################################
void SpriteBatcher::addBatch(shared_ptr<Sprite> batch)
{
	batches.push_back(batch);
}

void SpriteBatcher::draw()
{

	program->bind();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	sort(batches.begin(), batches.end(),
		[](shared_ptr<Sprite> a, shared_ptr<Sprite> b) -> bool
	    {
	      return a->layer < b->layer;
	    }
	);

	texture::Texture *current;

	cout << "---------------"<< "\n";

	for (auto batch : batches) {

		cout << batch->layer << "\n";

		glActiveTexture(GL_TEXTURE0);
		if (&batch->getTexture() != current) {
			current = &batch->getTexture();
			current->bind();
		}
		program->setUniformli("texture1", 0);

		glm::mat4 modelViewProjection = projection * batch->getTransform().getMatrix();

		program->setUniformMatrix4f("projection", modelViewProjection);
		batch->draw();
	}

	program->unbind();
	batches.clear();

}

void SpriteBatcher::setProjection(glm::mat4 projection)
{
	this->projection = projection;
}


} /* namespace graphics */
