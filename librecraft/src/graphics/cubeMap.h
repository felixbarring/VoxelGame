#ifndef SRC_GRAPHICS_CUBEMAP_H_
#define SRC_GRAPHICS_CUBEMAP_H_

#include "texture/textureCubeMap.h"
#include "camera.h"

namespace graphics {

class CubeMap {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    CubeMap(texture::TextureCubeMap &texture, Camera &camera);

// ########################################################
// Member Functions########################################
// ########################################################

    void draw();

// ########################################################
// Instance Variables #####################################
// ########################################################
private:

    texture::TextureCubeMap texture;
    Camera &m_camera;
};

}

#endif /* SRC_GRAPHICS_CUBEMAP_H_ */
