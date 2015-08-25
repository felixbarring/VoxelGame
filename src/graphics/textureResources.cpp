
#include "textureResources.h"

#include <iostream>

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

TextureResources::TextureResources()
{
	textureArrays.push_back({config::cube_data::textures, config::cube_data::TEXTURE_WIDTH, config::cube_data::TEXTURE_HEIGHT});

	// More to come here
}

TextureResources::~TextureResources()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

texture::Texture& TextureResources::getTexture(int value)
{
	if (value > textures.size() || value < 0) {
		std::cout << "Texture resources value is out of range! \n";
	}

	return textures.at(value);
}


texture::TextureArray& TextureResources::getTextureArray(int value)
{
	if (value > textureArrays.size() || value < 0) {
		std::cout << "Texture resources value is out of range! \n";
	}

	return textureArrays.at(value);
}

texture::TextureCubeMap& TextureResources::getTextureCubeMap(int value)
{
	if (value > textureCubeMaps.size() || value < 0) {
		std::cout << "Texture resources value is out of range! \n";
	}

	return textureCubeMaps.at(value);
}


} /* namespace graphics */
