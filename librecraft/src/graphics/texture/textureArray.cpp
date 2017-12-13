#include "textureArray.h"

#include <SFML/Graphics/Image.hpp>
#include <iostream>

using namespace sf;

namespace texture {

// TODO Clean up this class

TextureArray::TextureArray(std::vector<std::string> paths,
                           unsigned width,
                           unsigned height) {
  GLsizei layerCount = paths.size();

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);

  // Add more different Texture filtering options, the user should be able to
  // configure this
  // GL_LINEAR_MIPMAP_LINEAR
  // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,
  // GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
  // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glTexImage3D(GL_TEXTURE_2D_ARRAY,
               0,
               GL_RGBA8,
               width,
               height,
               layerCount,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               nullptr);

  Image image;

  int i = 0;
  for (auto path : paths) {
    if (!image.loadFromFile(path)) {
      std::cout << "Coulnd not load image" << path << "\n";
      return;
    }
    if (image.getSize().x != width || image.getSize().y != height) {
      std::cout << "Wrong size of the image" << path << "\n";
      return;
    }

    image.flipVertically();

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                    0,
                    0,
                    0,
                    i++,
                    width,
                    height,
                    1,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    image.getPixelsPtr());
  }

  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void
TextureArray::bind() const {
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
}
}
