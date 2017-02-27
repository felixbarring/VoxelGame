
#include "sprite.h"

namespace graphics {

Sprite::Sprite(double x, double y, unsigned layer, double width, double height, texture::Texture &texture)
    : m_transform{x + width / 2, y + height / 2, 0},
      m_width{width},
      m_height{height},
      m_texture(texture),
      m_layer{layer}
{

    std::vector<GLfloat> vertices = {
        static_cast<float>(-width/2.0), static_cast<float>(-height/2.0), 0.0f,
        static_cast<float>(width/2.0), static_cast<float>(-height/2.0), 0.0f,
        static_cast<float>(width/2.0), static_cast<float>(height/2.0), 0.0f,
        static_cast<float>(-width/2.0), static_cast<float>(height/2.0), 0.0f
    };

    std::vector<GLfloat> texCoords = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    std::vector<GLshort> indices = {
        0, 1, 2,
        0, 2, 3,
    };

    m_mesh.reset(new mesh::MeshElement(vertices, 3, texCoords, 2, indices));

}

Sprite::Sprite(double x, double y, unsigned layer, std::shared_ptr<mesh::MeshElement> mesh, texture::Texture &texture)
    : m_transform{x, y, 0},
      m_texture(texture),
      m_layer{layer}
{
	m_mesh = mesh;
}

void Sprite::draw() {
    m_mesh->draw();
}

Transform& Sprite::getTransform() {
    return m_transform;
}

texture::Texture& Sprite::getTexture() {
    return m_texture;
}

void Sprite::move(float x, float y) {
    m_transform.translate(x, y, 0);
}

void Sprite::setLocation(float x, float y) {
    m_transform.setLocation(x + m_width / 2, y + m_height / 2, 0);
}

int Sprite::getLayer() {
    return m_layer;
}


} /* namespace graphics */
