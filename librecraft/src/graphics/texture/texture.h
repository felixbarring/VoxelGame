#ifndef SRC_GRAPHICS_TEXTURE_H_
#define SRC_GRAPHICS_TEXTURE_H_

#include <GL/glew.h>

namespace texture {

class Texture {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    Texture(const char *path);

// ########################################################
// Member Functions########################################
// ########################################################

    void bind() const;

    // TODO
    // Add functions for texture options

// ########################################################
// Implementation #########################################
// ########################################################

private:
    GLuint textureID;

};

}

#endif /* SRC_GRAPHICS_TEXTURE_H_ */
