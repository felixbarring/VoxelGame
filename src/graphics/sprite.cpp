
#include "sprite.h"

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Sprite::Sprite(float x, float y, float layer, float width, float height, texture::Texture &texture) :
	transform{x + width / 2, y + height / 2, 0},
	texture(texture),
	layer{layer}
{

	std::vector<GLfloat> vertices = {
		-width/2, -height/2, 0.0f,
		width/2, -height/2, 0.0f,
		width/2, height/2, 0.0f,
		-width/2, height/2, 0.0f,
	};

	std::vector<GLfloat> texCoords = {

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

	};

	std::vector<GLshort> indices = {
		0, 1, 2,
		0, 2, 3,
	};

	mesh.reset(new mesh::MeshElement(vertices, 3, texCoords, 2, indices));

}

Sprite::Sprite(float x, float y, float z, std::shared_ptr<mesh::MeshElement> mesh, texture::Texture &texture) :
	transform{x, y, z},
	texture(texture),
	layer{y}
{
	this->mesh = mesh;
}

// ########################################################
// Member Functions########################################
// ########################################################

void Sprite::draw()
{
	mesh->draw();
}

Transform& Sprite::getTransform()
{
	return transform;
}

texture::Texture& Sprite::getTexture()
{
	return texture;
}

void Sprite::move(float x, float y)
{
	transform.translate(x, y, 0);
}

void Sprite::setLocation(float x, float y, float z)
{
	transform.setLocation(x, y, z);
}


} /* namespace graphics */
