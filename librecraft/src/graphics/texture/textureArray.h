#ifndef SRC_GRAPHICS_TEXTUREARRAY_H_
#define SRC_GRAPHICS_TEXTUREARRAY_H_

#include <GL/glew.h>
#include <string>
#include <vector>

namespace texture {

class TextureArray {
public:
  TextureArray(std::vector<std::string> paths, unsigned width, unsigned height);

  void bind() const;

private:
  GLuint textureID;
};
}

#endif /* SRC_GRAPHICS_ARRAYTEXTURE_H_ */
