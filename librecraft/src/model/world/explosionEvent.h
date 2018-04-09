#ifndef SRC_MODEL_WORLD_EXPLOSIONEVENT_H_
#define SRC_MODEL_WORLD_EXPLOSIONEVENT_H_

#include <iostream>

#include <glm/detail/type_vec.hpp>

#include "../../graphics/graphicsManager.h"
#include "../../graphics/transform.h"
#include "explosions.h"

namespace kabom {

class ExplosionEvent {
public:
  ExplosionEvent(glm::vec3 location,
                 kabom::Explosions&& explosion,
                 graphics::GraphicsManager& graphicsManager)
    : m_location{location}
    , m_explosion{explosion}
    , m_graphicsManager{graphicsManager} {
  }

  void update(double tick) {
    m_timePassedExplosion += tick;
    m_timePassedAnimation += tick;

    if (m_timePassedAnimation >= m_animationTime) {
      m_drawCube = !m_drawCube;
      m_timePassedAnimation = 0.0;
    }
    if (m_drawCube) {
      m_transform = graphics::Transform{
        m_location.x + 0.5, m_location.y + 0.5, m_location.z + 0.5};
      m_graphicsManager.getCubeBatcher().addBatch(
        config::cube_data::TNT,
        m_transform,
        config::graphics_data::directSunlightValue,
        config::graphics_data::directSunlightValue);
    } else {
      static const double lowLightValue{3};

      m_transform = graphics::Transform{
        m_location.x + 0.5, m_location.y + 0.5, m_location.z + 0.5};
      m_graphicsManager.getCubeBatcher().addBatch(
        config::cube_data::TNT, m_transform, lowLightValue, lowLightValue);
    }

    if (m_timePassedExplosion >= m_explosionTime) {
      m_explosion.explode(m_location);
      m_done = true;
    }
  }

  bool isDone() {
    return m_done;
  }

private:
  double m_timePassedExplosion{0.0};
  double m_timePassedAnimation{0.0};
  const double m_animationTime{0.1};
  const double m_explosionTime{5.0};

  bool m_drawCube{true};
  bool m_done{false};

  glm::vec3 m_location{};
  kabom::Explosions m_explosion;
  graphics::GraphicsManager& m_graphicsManager;

  graphics::Transform m_transform{0, 0, 0};
};
}

#endif /* SRC_MODEL_WORLD_EXPLOSIONEVENT_H_ */
