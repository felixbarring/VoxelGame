
#include "sprite.h"

#include <memory>

using std::make_unique;
namespace graphics {

using namespace std;

Sprite::Sprite(double x, double y, unsigned layer, double width, double height,
    texture::Texture &texture)
  : m_transform{x + width / 2, y + height / 2, 0}
  , m_width{width}
  , m_height{height}
  , m_texture(texture)
  , m_layer{layer}
{

  vector<GLfloat> vertices = {
    static_cast<float>(-width/2.0), static_cast<float>(-height/2.0), 0.0f,
    static_cast<float>(width/2.0), static_cast<float>(-height/2.0), 0.0f,
    static_cast<float>(width/2.0), static_cast<float>(height/2.0), 0.0f,
    static_cast<float>(-width/2.0), static_cast<float>(height/2.0), 0.0f
  };

  vector<GLfloat> texCoords = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
  };

  vector<GLshort> elementData = {
    0, 1, 2,
    0, 2, 3,
  };

  vector<pair<vector<float>, int>> vbos{{vertices, 3}, {texCoords, 2}};
  m_mesh = make_unique<mesh::MeshElement>(std::move(vbos), elementData);
}

Sprite::Sprite(double x, double y, unsigned layer,
    shared_ptr<mesh::MeshElement> mesh, texture::Texture &texture)
  : m_transform{x, y, 0}
  , m_texture(texture)
  , m_layer{layer}
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
