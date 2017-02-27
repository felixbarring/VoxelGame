#ifndef SRC_GRAPHICS_TEXTURE_H_
#define SRC_GRAPHICS_TEXTURE_H_

#include <GL/glew.h>

namespace texture {

class Texture {
public:

    Texture(const char *path);

    void bind() const;

    // TODO
    // Add functions for texture options

private:
    GLuint textureID;

};

}

#endif /* SRC_GRAPHICS_TEXTURE_H_ */
