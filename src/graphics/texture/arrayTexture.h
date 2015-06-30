/*
 * arrayTexture.h
 *
 *  Created on: Jun 29, 2015
 *      Author: felix
 */

#ifndef SRC_GRAPHICS_ARRAYTEXTURE_H_
#define SRC_GRAPHICS_ARRAYTEXTURE_H_

#include <GL/glew.h>

#include <vector>
#include <string>

class ArrayTexture {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	ArrayTexture(std::vector<std::string> paths, int width, int height);
	virtual ~ArrayTexture();

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
