#include "texture.h"

#include <SFML/Graphics/Image.hpp>
#include <iostream>

using namespace sf;

namespace texture {

// TODO Replace with C++ strings instead of c raw pointer strings.

Texture::Texture(const char* path) {
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(
    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  Image image;
  if (!image.loadFromFile(path)) {
    std::cout << "Coulnd not load image" << path << "\n";
    return;
  }
  image.flipVertically();

  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               image.getSize().x,
               image.getSize().y,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               image.getPixelsPtr());
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void
Texture::bind() const {
  glBindTexture(GL_TEXTURE_2D, textureID);
}
}
