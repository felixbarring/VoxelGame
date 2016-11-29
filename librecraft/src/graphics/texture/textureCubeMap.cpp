#include "textureCubeMap.h"

#include <iostream>
#include <SFML/Graphics/Image.hpp>

using namespace sf;

namespace texture {

static GLuint targets[] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

// ########################################################
// Constructor/Destructor #################################
// ########################################################

TextureCubeMap::TextureCubeMap(const char* right, const char* left,
		const char* top, const char* bottom, const char* back,
		const char* front, unsigned width, unsigned height) {

    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::vector<const char*> paths{right, left, top, bottom, back, front};

    static Image *image = new Image;

    int i = 0;
    for (auto path : paths) {

        if (!image->loadFromFile(path)) {
            std::cout << "Coulnd not load image" << path << "\n";
            return;
        }

        glTexImage2D(targets[i++], 0, GL_RGBA, image->getSize().x, image->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                image->getPixelsPtr());
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

// ########################################################
// Member Functions########################################
// ########################################################

void TextureCubeMap::bind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

}
