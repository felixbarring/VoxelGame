#ifndef GAME_MODEL_ENTITIES_CUBE_H_
#define GAME_MODEL_ENTITIES_CUBE_H_

#include <memory>

#include "mesh/meshElement.h"
#include "transform.h"

namespace graphics {

class TexturedCube {
public:
  TexturedCube(float xOffset, float yOffset, float zOffset, int id);

  void draw();

  float getxLocation() {
    return xLocation;
  }

  float getyLocation() {
    return yLocation;
  }

private:
  // unique pointer gives error?!?
  std::shared_ptr<mesh::MeshElement> m_mesh;

  float xLocation;
  float yLocation;
  float zLocation;
};
}

#endif /* GAME_MODEL_ENTITIES_CUBE_H_ */
