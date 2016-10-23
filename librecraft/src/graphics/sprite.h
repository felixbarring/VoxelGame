#ifndef SRC_GRAPHICS_SPRITE_H_
#define SRC_GRAPHICS_SPRITE_H_

#include <memory>

#include "transform.h"
#include "texture/texture.h"
#include "texture/texture.h"
#include "mesh/meshElement.h"

namespace graphics {

class Sprite {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    Sprite(float x, float y, float layer, float width, float height, texture::Texture &texture);

    Sprite(float x, float y, float layer, std::shared_ptr<mesh::MeshElement> mesh, texture::Texture &texture);

    virtual ~Sprite() {};

// ########################################################
// Member Functions########################################
// ########################################################

    void draw();

    Transform& getTransform();

    texture::Texture& getTexture();

    void move(float x, float y);

    void setLocation(float x, float y, float z);

    int getLayer();

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

    float m_width{};
    float m_height{};

    std::shared_ptr<mesh::MeshElement> m_mesh;
    texture::Texture &m_texture;
    Transform m_transform;
    int m_layer;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_SPRITE_H_ */
