
#ifndef SRC_GRAPHICS_TEXTURERESOURCES_H_
#define SRC_GRAPHICS_TEXTURERESOURCES_H_

#include <vector>

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

	texture::Texture& getTexture(int value);

	texture::TextureArray& getTextureArray(int value);

	texture::TextureCubeMap& getTextureCubeMap(int value);

// ########################################################
// Instance Variables #####################################
// ########################################################

	std::vector<texture::Texture> textures;
	std::vector<texture::TextureArray> textureArrays;
	std::vector<texture::TextureCubeMap> textureCubeMaps;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_TEXTURERESOURCES_H_ */
