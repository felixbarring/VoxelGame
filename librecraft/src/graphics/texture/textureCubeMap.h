#ifndef SRC_GRAPHICS_TEXTURE_TEXTURECUBEMAP_H_
#define SRC_GRAPHICS_TEXTURE_TEXTURECUBEMAP_H_

#include <GL/glew.h>
#include <vector>
#include <string>

namespace texture {

class TextureCubeMap {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    // TODO Take all six paths as arguments!

    TextureCubeMap(const char* right, const char* left, const char* top,
            const char* bottom, const char* back, const char* front,
            unsigned width, unsigned height);

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

#endif /* SRC_GRAPHICS_TEXTURE_TEXTURECUBEMAP_H_ */
