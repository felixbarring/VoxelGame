
#include "arrayTexture.h"

#include <vector>
#include <string>

#include <SOIL.h>

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ArrayTexture::ArrayTexture(std::vector<std::string> paths, int width, int height)
{
	GLsizei layerCount = paths.size();

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, layerCount, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	int i = 0;
	for (auto path : paths) {
		unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i++, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
}

ArrayTexture::~ArrayTexture()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void ArrayTexture::bind()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
}

