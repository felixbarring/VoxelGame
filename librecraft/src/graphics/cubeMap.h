#ifndef SRC_GRAPHICS_CUBEMAP_H_
#define SRC_GRAPHICS_CUBEMAP_H_

#include "texture/textureCubeMap.h"
#include "camera.h"

namespace graphics {

class CubeMap {
public:

    CubeMap(texture::TextureCubeMap &texture, Camera &camera);

    void setRotationValue(float value);

    void draw(double trancparency);

private:

    texture::TextureCubeMap m_texture;
    Camera &m_camera;
    float m_roatationValue{0.0f};
};

}

#endif /* SRC_GRAPHICS_CUBEMAP_H_ */
