
#include "textureArray.h"

#include <vector>
#include <string>
#include <SOIL.h>

// ########################################################
// Constructor/Destructor #################################
// ########################################################

TextureArray::TextureArray(std::vector<std::string> paths, int width, int height)
{
	GLsizei layerCount = paths.size();

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);

	// Add more different Texture filtering options, the user should be able to configure this

	// GL_LINEAR_MIPMAP_LINEAR
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, layerCount, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	int i = 0;
	for (auto path : paths) {

		int w;
		int h;
		unsigned char* image = SOIL_load_image(path.c_str(), &w, &h, 0, SOIL_LOAD_RGB);

		if (w != width || h != height) {
			// ERROR HERE :(
		}

		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i++, w, h, 1, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

TextureArray::~TextureArray()
{
	// TODO Delete texture from opengl
}

// ########################################################
// Member Functions########################################
// ########################################################

void TextureArray::bind()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
}

