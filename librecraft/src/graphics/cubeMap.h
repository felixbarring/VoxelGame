#ifndef SRC_GRAPHICS_CUBEMAP_H_
#define SRC_GRAPHICS_CUBEMAP_H_

#include <memory>

#include "camera.h"
#include "mesh/meshElement.h"
#include "shaderProgram.h"
#include "texture/textureCubeMap.h"

namespace graphics {

class CubeMap {
public:
  CubeMap(texture::TextureCubeMap& texture, Camera& camera);

  void setRotationValue(float value);

  /**
   *
   * @param transparency
   */
  void draw(double transparency);

private:
  texture::TextureCubeMap m_texture;
  Camera& m_camera;
  float m_roatationValue{ 0.0f };

  std::unique_ptr<graphics::ShaderProgram> m_program;
  std::unique_ptr<mesh::MeshElement> mesh;
};
}

#endif /* SRC_GRAPHICS_CUBEMAP_H_ */
