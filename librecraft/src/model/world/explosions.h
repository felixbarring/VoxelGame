
#ifndef SOURCE_DIRECTORY__SRC_MODEL_WORLD_EXPLOSIONS_H_
#define SOURCE_DIRECTORY__SRC_MODEL_WORLD_EXPLOSIONS_H_

#include "chunk/chunkManager.h"
#include <glm/detail/type_vec.hpp>
#include <vector>

using chunk::ChunkManager;

// TODO Fix a better namspace name.
namespace kabom {

class Explosions {
public:
  Explosions(int radius, chunk::ChunkManager& chunkManager)
    : m_radius{radius}
    , m_chunkManager{chunkManager} {
  }

  void explode(glm::vec3 point) {

    // Remove all cubes in a sphere.

    int x{static_cast<int>(point.x)};
    int y{static_cast<int>(point.y)};
    int z{static_cast<int>(point.z)};

    std::vector<glm::vec3> candidates;
    for (int i{x - m_radius}; i < x + m_radius; ++i) {
      for (int j{y - m_radius}; j < y + m_radius; ++j) {
        for (int k{z - m_radius}; k < z + m_radius; ++k) {
          candidates.push_back({i, j, k});
        }
      }
    }

    for (glm::vec3& candidate : candidates) {
      if (glm::length(candidate - point) < m_radius) {
        m_chunkManager.removeCube(candidate.x, candidate.y, candidate.z);
      }
    }

    // TODO Start graphics explosion here.
  }

private:
  int m_radius;
  chunk::ChunkManager& m_chunkManager;
};
}

#endif /* SOURCE_DIRECTORY__SRC_MODEL_WORLD_EXPLOSIONS_H_ */
