
#ifndef SRC_GRAPHICS_TEXTURE_H_
#define SRC_GRAPHICS_TEXTURE_H_

#include <GL/glew.h>

namespace texture
{

class Texture {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	Texture(const char *path);
	virtual ~Texture();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	void bind();

	// TODO
	// Add functions for texture options

	// ########################################################
	// Instance Variables #####################################
	// ########################################################
private:
	GLuint textureID;

};

}

#endif /* SRC_GRAPHICS_TEXTURE_H_ */
