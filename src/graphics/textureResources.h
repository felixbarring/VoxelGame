
#ifndef SRC_GRAPHICS_TEXTURERESOURCES_H_
#define SRC_GRAPHICS_TEXTURERESOURCES_H_

#include <vector>
#include <map>
#include <string>

#include "../config/data.h"

#include "texture/texture.h"
#include "texture/textureArray.h"
#include "texture/textureCubeMap.h"

namespace graphics {

class TextureResources {
private:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	TextureResources();

	virtual ~TextureResources();

	TextureResources(TextureResources const&) = delete;

	void operator=(TextureResources const&) = delete;

public:

	static TextureResources& getInstance()
	{
		static TextureResources instance;
		return instance;
	}

// ########################################################
// Member Functions########################################
// ########################################################

	texture::Texture& getTexture(const std::string &path);

	texture::TextureArray& getTextureArray(const std::vector<std::string> &textures, int width, int height);

	texture::TextureCubeMap& getTextureCubeMap(const char* right, const char* left, const char* top,
			const char* bottom, const char* back, const char* front,  int width, int height);

	texture::TextureCubeMap& getTextureCubeMap(int value);


// ########################################################
// Instance Variables #####################################
// ########################################################

	std::map<std::string, texture::Texture> textures;

	std::vector<texture::TextureCubeMap> textureCubeMaps;

	std::map<std::string, texture::TextureArray> textureArraysMap;
	std::map<std::string, texture::TextureCubeMap> textureCubeMapsMap;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_TEXTURERESOURCES_H_ */
