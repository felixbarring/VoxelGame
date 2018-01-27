#ifndef GAME_MODEL_ENTITIES_CUBE_H_
#define GAME_MODEL_ENTITIES_CUBE_H_

#include <memory>

#include "mesh/meshElement.h"
#include "transform.h"

namespace graphics {

class TexturedCube {
public:
  explicit TexturedCube(int id);

  void draw();

private:
  std::unique_ptr<mesh::MeshElement> m_mesh;
};
}

#endif /* GAME_MODEL_ENTITIES_CUBE_H_ */
