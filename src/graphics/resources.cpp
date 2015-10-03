
#include "resources.h"

#include <iostream>

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

texture::Texture& Resources::getTexture(const std::string &path)
{

	std::map<std::string, texture::Texture>::iterator it = textures.find(path);

	if (it == textures.end()) {
		textures.insert(std::make_pair(path, texture::Texture{path.c_str()}));
	}

	return textures.at(path);
}

texture::TextureArray& Resources::getTextureArray(const std::vector<std::string> &textures, int width, int height)
{
	std::map<std::string, texture::TextureArray> ::iterator it = textureArraysMap.find(textures.at(0));

	if (it == textureArraysMap.end()) {
		textureArraysMap.insert(std::make_pair(textures.at(0), texture::TextureArray{textures, width, height}));
	}

	return textureArraysMap.at(textures.at(0));
}

texture::TextureCubeMap& Resources::getTextureCubeMap(std::string &right, std::string &left, std::string &top,
		std::string &bottom, std::string &back, std::string &front,  int width, int height)
{
	std::map<std::string, texture::TextureCubeMap>::iterator it = textureCubeMaps.find(right);

	if (it == textureCubeMaps.end()) {
		textureCubeMaps.insert(std::make_pair(right, texture::TextureCubeMap{
			right.c_str(), left.c_str(), top.c_str(), bottom.c_str(), back.c_str(), front.c_str(), width, height}));
	}

	return textureCubeMaps.at(right);
}

FontMeshBuilder& Resources::getFontMeshBuilder(std::string &pathToLayout, int atlasWidth, int atlasHeight)
{
	std::map<std::string, FontMeshBuilder> ::iterator it = fontMeshBuilders.find(pathToLayout);

	if (it == fontMeshBuilders.end()) {
		fontMeshBuilders.insert(std::make_pair(pathToLayout, FontMeshBuilder{pathToLayout, atlasWidth, atlasHeight}));
	}

	return fontMeshBuilders.at(pathToLayout);
}

} /* namespace graphics */
