#include "texture.h"

#include <iostream>
#include <SFML/Graphics/Image.hpp>

using namespace sf;

namespace texture {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Texture::Texture(const char *path) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	static Image *image = new Image;
	if(!image->loadFromFile(path)) {
		std::cout << "Coulnd not load image" << path << "\n";
		return;
	}
	image->flipVertically();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			image->getSize().x,
			image->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			image->getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}


// ########################################################
// Member Functions########################################
// ########################################################

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, textureID);
}

}
