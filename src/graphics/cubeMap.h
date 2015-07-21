
#ifndef SRC_GRAPHICS_CUBEMAP_H_
#define SRC_GRAPHICS_CUBEMAP_H_

#include "texture/textureCubeMap.h"
#include "camera.h"

class CubeMap {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	CubeMap(const char* right, const char* left, const char* top, const char* bottom, const char* back, const char* front, int widht, int height);
	virtual ~CubeMap();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	void render(Camera &camera);

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	TextureCubeMap texture;

};

#endif /* SRC_GRAPHICS_CUBEMAP_H_ */
