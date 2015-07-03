
#ifndef SRC_GRAPHICS_TEXTUREARRAY_H_
#define SRC_GRAPHICS_TEXTUREARRAY_H_

#include <GL/glew.h>

#include <vector>
#include <string>

class TextureArray {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	TextureArray(std::vector<std::string> paths, int width, int height);
	virtual ~TextureArray();

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

#endif /* SRC_GRAPHICS_ARRAYTEXTURE_H_ */
