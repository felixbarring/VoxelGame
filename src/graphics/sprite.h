
#ifndef SRC_GRAPHICS_SPRITE_H_
#define SRC_GRAPHICS_SPRITE_H_

#include <memory>

#include "transform.h"
#include "texture/texture.h"
#include "mesh/meshElement.h"

namespace graphics {

class Sprite {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Sprite(float x, float y, float z, float width, float height, texture::Texture &texture);

	virtual ~Sprite();

// ########################################################
// Member Functions########################################
// ########################################################

	void draw();

	Transform& getTransform();

	texture::Texture& getTexture();

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	std::unique_ptr<mesh::MeshElement> mesh;
	texture::Texture &texture;
	Transform transform;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_SPRITE_H_ */
