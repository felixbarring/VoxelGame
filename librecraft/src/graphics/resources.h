
#ifndef SRC_GRAPHICS_RESOURCES_H_
#define SRC_GRAPHICS_RESOURCES_H_

#include <vector>
#include <map>
#include <string>

#include "../config/data.h"

#include "texture/texture.h"
#include "texture/textureArray.h"
#include "texture/textureCubeMap.h"

#include "fontMeshBuilder.h"

namespace graphics {

class Resources {
private:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Resources() {};

	virtual ~Resources() {};

	Resources(Resources const&) = delete;

	void operator=(Resources const&) = delete;

public:

	static Resources& getInstance()
	{
		static Resources instance;
		return instance;
	}

// ########################################################
// Member Functions########################################
// ########################################################

	texture::Texture& getTexture(const std::string &path);

	texture::TextureArray& getTextureArray(
			const std::vector<std::string> &textures, int width, int height);

	texture::TextureCubeMap& getTextureCubeMap(
			std::string &right, std::string &left, std::string &top,
			std::string &bottom, std::string &back, std::string &front,
			int width, int height);

	texture::TextureCubeMap& getTextureCubeMap(int value);

	FontMeshBuilder& getFontMeshBuilder(
			std::string &pathToLayout, int atlasWidth, int atlasHeight);


// ########################################################
// Instance Variables #####################################
// ########################################################

	std::map<std::string, texture::Texture> textures;
	std::map<std::string, texture::TextureArray> textureArraysMap;
	std::map<std::string, texture::TextureCubeMap> textureCubeMaps;
	std::map<std::string, FontMeshBuilder> fontMeshBuilders;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_RESOURCES_H_ */
