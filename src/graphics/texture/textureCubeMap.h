
#ifndef SRC_GRAPHICS_TEXTURE_TEXTURECUBEMAP_H_
#define SRC_GRAPHICS_TEXTURE_TEXTURECUBEMAP_H_

#include <GL/glew.h>
#include <vector>
#include <string>

class TextureCubeMap {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	// TODO Take all six paths as arguments!

	TextureCubeMap(const std::vector<std::string> &paths, int width, int height);
	virtual ~TextureCubeMap();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	void bind();

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

private:
	GLuint textureID;

};

#endif /* SRC_GRAPHICS_TEXTURE_TEXTURECUBEMAP_H_ */
