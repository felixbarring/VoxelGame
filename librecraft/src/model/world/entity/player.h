#ifndef SRC_MODEL_WORLD_ENTITY_PLAYER_H_
#define SRC_MODEL_WORLD_ENTITY_PLAYER_H_

#include <glm/glm.hpp>

#include <vector>

#include "graphics/transform.h"
#include "graphics/viewDirection.h"
#include "util/input.h"
#include "ientity.h"

#include "config/data.h"
#include "graphics/graphicsManager.h"

#include "model/world/chunk/chunkManager.h"
#include "model/world/explosionEvent.h"

#include "footStepSoundPlayer.h"

namespace entity {

class Player : public IEntity {
public:
  Player(chunk::ChunkManager& chunkManager,
         util::SoundPlayer& soundPlayer,
         graphics::GraphicsManager& graphicsManager);

  void update(float timePassed) override;

  void setLocation(float x, float y, float z);

  void turnGravityOff(bool value = true);

  glm::vec3 getViewingDirection();

  glm::vec3 getLastSelectedCube();

  int getBuildingCube() {
    return m_cubeUsedForBuilding;
  }

  void setSpeed(double value);

  void setLimit(const AABB& limit);

private:
  void updateSpeed(float timePassed);

  void handlePhysics();

  void updateCameraAndTargetCube();

  void updateIsOnGround();

  void intersected(glm::vec3 movement,
                   std::vector<std::tuple<float, int, glm::vec3>>& collisions);

  bool isInWater();

  entity::AABB createAABB();

  const float m_width{0.8};
  const float m_height{1.7};
  const float m_depth{0.8};
  const float m_cameraHeight{1.5};

  float m_gravity{30};
  float m_jumpSpeed{12};
  int m_selectCubeDistance{5};
  float m_movementSpeed{8};

  bool m_gravitiyOn{true};
  bool m_isOnGround{false};

  float m_flySpeed{8};

  glm::vec3 m_location{0, 0, 0}; // The location of the camera

  chunk::ChunkManager& m_chunkManager;
  FootStepSoundPlayer m_stepPlayer;
  graphics::GraphicsManager& m_graphicsManager;
  util::SoundPlayer& m_soundPlayer;

  glm::vec3 m_speed{0, 0, 0};
  glm::vec3 m_frameSpeed{};
  glm::vec3 m_lastSelecteCube{};
  graphics::ViewDirection m_viewDirection{};
  graphics::Transform m_targetedCubeTransform{0, 0, 0};
  int m_cubeUsedForBuilding{0};

  const AABB* m_limit{};

  std::vector<kabom::ExplosionEvent> m_explosionEvent{};
};

} /* namespace entity */

#endif /* SRC_MODEL_WORLD_ENTITY_PLAYER_H_ */
