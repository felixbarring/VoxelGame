#ifndef SRC_GRAPHICS_TEXTUREARRAY_H_
#define SRC_GRAPHICS_TEXTUREARRAY_H_

#include <GL/glew.h>
#include <vector>
#include <string>

namespace texture {

class TextureArray {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    TextureArray(std::vector<std::string> paths, unsigned width, unsigned height);

// ########################################################
// Member Functions########################################
// ########################################################

    void bind() const;

// ########################################################
// Implementation #########################################
// ########################################################

private:
    GLuint textureID;

};

}

#endif /* SRC_GRAPHICS_ARRAYTEXTURE_H_ */
