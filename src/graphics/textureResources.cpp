
#include "textureResources.h"

#include <iostream>

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

TextureResources::TextureResources()
{
	//textureArrays.push_back({config::cube_data::textures, config::cube_data::TEXTURE_WIDTH, config::cube_data::TEXTURE_HEIGHT});
}

TextureResources::~TextureResources()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

texture::Texture& TextureResources::getTexture(const std::string &path)
{

	std::map<std::string, texture::Texture>::iterator it = textures.find(path);

	if (it == textures.end()) {
		textures.insert(std::make_pair(path, texture::Texture{path.c_str()}));
	}

	return textures.at(path);
}

texture::TextureCubeMap& TextureResources::getTextureCubeMap(int value)
{
	if (value > textureCubeMaps.size() || value < 0) {
		std::cout << "Texture resources value is out of range! \n";
	}

	return textureCubeMaps.at(value);
}

texture::TextureArray& TextureResources::getTextureArray(const std::vector<std::string> &textures, int width, int height)
{
	std::map<std::string, texture::TextureArray> ::iterator it = textureArraysMap.find(textures.at(0));

	if (it == textureArraysMap.end()) {
		textureArraysMap.insert(std::make_pair(textures.at(0), texture::TextureArray{textures, width, height}));
	}

	return textureArraysMap.at(textures.at(0));
}

// TODO Implement
texture::TextureCubeMap& TextureResources::getTextureCubeMap(const char* right, const char* left, const char* top,
		const char* bottom, const char* back, const char* front,  int width, int height)
{
	/*
	std::map<std::string, texture::Texture> ::iterator it = textureCubeMapsMap.at(path);

	if (it == textures.end()) {
		textureCubeMapsMap.insert(std::make_pair(path, texture::TextureCubeMap{path.c_str()}));
	}

	*/
	return textureCubeMaps.at(0);
}


} /* namespace graphics */
