#ifndef SRC_MODEL_WORLD_EXPLOSIONEVENT_H_
#define SRC_MODEL_WORLD_EXPLOSIONEVENT_H_

#include <iostream>
#include <vector>

#include <glm/detail/type_vec.hpp>
#include <string>

#include "graphics/graphicsManager.h"
#include "graphics/transform.h"
#include "config/data.h"

#include "chunk/chunkManager.h"

namespace kabom {


// Comment about supporting copy...
/**
 *
 *
 * @brief
 */
class ExplosionEvent {
public:

  /**
   * @brief
   *
   * @param location
   * @param radius
   * @param graphicsManager
   * @param chunkManager
   */
  ExplosionEvent(glm::vec3 location,
                 int radius,
                 graphics::GraphicsManager& graphicsManager,
                 chunk::ChunkManager& chunkManager,
                 util::SoundPlayer& soundPlayer)
    : m_location{location}
    , m_radius{radius}
    , m_graphicsManager{&graphicsManager}
    , m_chunkManager{&chunkManager}
    , m_soundPlayer{&soundPlayer} {
      m_soundPlayer->playSound(config::audio::fuse);
  }

  /**
   *
   * @param tick
   */
  void update(double tick) {

    if (m_done)
      return;

    m_timePassedExplosion += tick;
    m_timePassedAnimation += tick;

    if (m_timePassedAnimation >= m_animationTime) {
      m_drawCube = !m_drawCube;
      m_timePassedAnimation = 0.0;
    }
    if (m_drawCube) {
      m_transform = graphics::Transform{
        m_location.x + 0.5, m_location.y + 0.5, m_location.z + 0.5};
      m_graphicsManager->getCubeBatcher().addBatch(
        config::cube_data::TNT,
        m_transform,
        config::graphics_data::directSunlightValue,
        config::graphics_data::directSunlightValue);
    } else {
      static const double lowLightValue{3};

      m_transform = graphics::Transform{
        m_location.x + 0.5, m_location.y + 0.5, m_location.z + 0.5};
      m_graphicsManager->getCubeBatcher().addBatch(
        config::cube_data::TNT, m_transform, lowLightValue, lowLightValue);
    }

    if (m_timePassedExplosion >= m_explosionTime) {
      explode();
      m_soundPlayer->playSound(config::audio::explosion);
      m_done = true;
    }
  }

  /**
   *
   * @return
   */
  bool isDone() {
    return m_done;
  }

  glm::vec3 getLocation() {
    return m_location;
  }

private:

  void explode() {
    // Remove all cubes in a sphere.
    int x{static_cast<int>(m_location.x)};
    int y{static_cast<int>(m_location.y)};
    int z{static_cast<int>(m_location.z)};

    std::vector<glm::vec3> candidates;
    for (int i{x - m_radius}; i < x + m_radius; ++i) {
      for (int j{y - m_radius}; j < y + m_radius; ++j) {
        for (int k{z - m_radius}; k < z + m_radius; ++k) {
          candidates.push_back({i, j, k});
        }
      }
    }

    for (glm::vec3& candidate : candidates) {
      if (glm::length(candidate - m_location) < m_radius) {
        m_chunkManager->removeCube(candidate.x, candidate.y, candidate.z);
      }
    }

    // TODO Start graphics explosion here.
  }

  double m_timePassedExplosion{0.0};
  double m_timePassedAnimation{0.0};
  static constexpr double m_animationTime{0.1};
  static constexpr double m_explosionTime{5.0};

  bool m_drawCube{true};
  bool m_done{false};

  glm::vec3 m_location;
  int m_radius;
  graphics::GraphicsManager* m_graphicsManager;
  chunk::ChunkManager* m_chunkManager;
  util::SoundPlayer* m_soundPlayer;
  graphics::Transform m_transform{0, 0, 0};
};
}

#endif /* SRC_MODEL_WORLD_EXPLOSIONEVENT_H_ */
