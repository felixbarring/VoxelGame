
#ifndef SRC_GRAPHICS_CUBEMAP_H_
#define SRC_GRAPHICS_CUBEMAP_H_

#include "texture/textureCubeMap.h"
#include "camera.h"

namespace graphics
{

class CubeMap {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	CubeMap(texture::TextureCubeMap &texture);

	virtual ~CubeMap() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void render();

// ########################################################
// Instance Variables #####################################
// ########################################################

	texture::TextureCubeMap texture;

};

}

#endif /* SRC_GRAPHICS_CUBEMAP_H_ */
