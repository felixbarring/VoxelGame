#include "player.h"

#include <iostream>
#include <math.h>
#include <memory>
#include <algorithm>

#include "../../../graphics/camera.h"
#include "../chunk/chunkManager.h"
#include "../../../graphics/cubeBatcher.h"
#include "../../../graphics/graphicsManager.h"
#include "../../../util/voxel.h"

#include "../../../util/soundPlayer.h"

#include "aabb.h"

using namespace std;
using namespace glm;

using namespace util;
using namespace graphics;
using namespace chunk;
using namespace config;
using namespace cube_data;

namespace entity {

Player::Player(chunk::ChunkManager& chunkManager,
    util::SoundPlayer &soundPlayer, graphics::GraphicsManager &graphicsManager)
  : m_chunkManager{chunkManager}
  , m_stepPlayer{soundPlayer, config::souds::footStepSounds}
  , m_graphicsManager{graphicsManager}
{
}

void Player::update(float timePassed) {
  updateIsOnGround();
  updateSpeed(timePassed);
  handlePhysics();
  updateCameraAndTargetCube(); // Updates the camera as well

  if (length(m_speed) && m_isOnGround)
    m_stepPlayer.walkingActive(timePassed);

  m_chunkManager.setCenter(m_location.x, m_location.z);
}

void Player::setLocation(float x, float y, float z) {
  m_location = vec3(x, y, z);
}

void Player::turnGravityOff(bool value) {
  m_gravitiyOn = !value;
}

vec3 Player::getViewingDirection() {
  return m_viewDirection.getViewDirection();
}

glm::vec3 Player::getLastSelectedCube() {
  return m_lastSelecteCube;
}

void Player::updateSpeed(float timePassed) {
  auto input = Input::getInstance();
  m_viewDirection.changeViewDirection(input->mouseXMovement,
      input->mouseYMovement);

  m_speed.x = 0;
  m_speed.z = 0;

  vec3 movementDirection{0.0f, 0.0f, 0.0f};

  if (input->moveForwardActive || input->moveBackwardActive) {
    float directionSign = 1.0f;
    if (input->moveBackwardActive)
      directionSign = -1.0f;

    vec3 direction = m_viewDirection.getViewDirection();
    direction.y = 0;
    movementDirection += directionSign * direction;
  }

  if (input->moveRightActive || input->moveLeftActive) {
    float direction = 1.0f;
    if (input->moveLeftActive)
      direction = -1.0f;

    vec3 dummy = m_viewDirection.getRightDirection();
    dummy.y = 0;
    movementDirection += direction * dummy;
  }

  vec3 normalizedMD = m_movementSpeed * normalize(movementDirection);

  // Normalize will give nan if the length is 0
  if (length(movementDirection)) {
    m_speed.x = normalizedMD.x;
    m_speed.z = normalizedMD.z;
  }

  if (input->switchCubePressed && ++m_cubeUsedForBuilding >
      config::cube_data::LAST_CUBE_USED_FOR_BUILDING)
    m_cubeUsedForBuilding = 0;

  float waterFactor = 1.0;

  if (m_gravitiyOn) {
    m_speed.y -= m_gravity * timePassed;

    if (input->jumpPressed) {
      if (m_isOnGround) {
        m_speed.y = m_jumpSpeed;
        m_stepPlayer.playSteppingSound();
      }
    }

    if (isInWater()) {
      waterFactor = 0.1;
      if (input->jumpActive || input->goDownActive) {
        int direction = 1;
        if (input->goDownActive)
          direction = -1;

        m_speed.y = 8 * direction;
      }
    }
  } else {
    // Gravity off...
    if (input->jumpActive || input->goDownActive) {
      int direction = 1;
      if (input->goDownActive)
        direction = -1;

      m_speed.y = m_flySpeed * direction;
    } else {
      m_speed.y = 0;
    }
  }

  vector<tuple<float, int, vec3>> collisions;
  intersected(vec3(0, 0, 0), collisions);

  m_frameSpeed = m_speed * waterFactor * timePassed;
}

void Player::handlePhysics() {
  vector<tuple<float, int, vec3>> collisions;
  intersected(m_frameSpeed, collisions);

  while (collisions.size()) {

    sort(collisions.begin(), collisions.end(),
      [](tuple<float, int, vec3> a, tuple<float, int, vec3> b)
      {
        return get<0>(a) < get<0>(b);
      });

//        auto c = collisions[0];
//        auto time = get<0>(collisions[0]);

    float derp = 1.0; // / 5.0;
    auto separationVec = derp * -std::get<2>(collisions[0]);

    // TODO Fix the incorrect collision responses...
    // When colliding, the entire speed in the direction of the collision normal is removed
    // Should be less.

    m_frameSpeed += vec3(separationVec.x * m_frameSpeed.x,
        separationVec.y * m_frameSpeed.y, separationVec.z * m_frameSpeed.z);

    m_speed += vec3(separationVec.x * m_speed.x, separationVec.y * m_speed.y,
        separationVec.z * m_speed.z);

    collisions.clear();
    intersected(m_frameSpeed, collisions);
  }
  m_location += m_frameSpeed;

}

void Player::updateCameraAndTargetCube() {
  shared_ptr<Input> input = Input::getInstance();

  m_graphicsManager.getPlayerCamera().updateView(
      vec3(m_location.x, m_location.y, m_location.z),
      m_viewDirection.getViewDirection(), m_viewDirection.getUpDirection());

  vec3 selectedCube;
  vec3 previous;

  if (m_chunkManager.intersectWithSolidCube(m_location,
      m_viewDirection.getViewDirection(), selectedCube, previous,
      m_selectCubeDistance)) {

    m_lastSelecteCube = selectedCube;

    if (input->action1Pressed) {
      m_chunkManager.removeCube(selectedCube.x, selectedCube.y, selectedCube.z);
      return;
    } else if (input->action2Pressed) {
      AABB playerAAABB = createAABB();
      AABB cubeAABB{previous.x, previous.x + 1, previous.y, previous.y + 1,
          previous.z, previous.z + 1};
      if (!playerAAABB.intersects(cubeAABB))
        m_chunkManager.setCube(previous.x, previous.y, previous.z,
            m_cubeUsedForBuilding);
      return;
    }

    // TODO Remove hardcoded values
    // 0.5 = half width of cube
    m_targetedCubeTransform.setLocation(selectedCube.x + 0.5,
        selectedCube.y + 0.5, selectedCube.z + 0.5);

    char voxelID = m_chunkManager.getCubeId(selectedCube.x, selectedCube.y,
        selectedCube.z);
    char voxelLightValue = m_chunkManager.getVoxel(previous.x, previous.y,
        previous.z).lightValue;
    m_graphicsManager.getCubeBatcher().addBatch(voxelID,
        m_targetedCubeTransform, voxelLightValue + 5);
  }

}

void Player::updateIsOnGround() {
  vector<tuple<float, int, vec3>> collisions;
  intersected(vec3(0, -0.1, 0), collisions);
  m_isOnGround = !collisions.empty();
}

void Player::intersected(vec3 movement,
                         vector<tuple<float, int, vec3>> &collisions) {

  AABB start = createAABB();

  AABB box = AABB::getSweptBroadPhaseBox(start, movement);

  int xStart = floor(box.m_xMin);
  int yStart = floor(box.m_yMin);
  int zStart = floor(box.m_zMin);

  int xEnd = floor(box.m_xMax);
  int yEnd = floor(box.m_yMax);
  int zEnd = floor(box.m_zMax);

  for (double i = xStart; i <= xEnd; ++i) {
    for (double j = yStart; j <= yEnd; ++j) {
      for (double k = zStart; k <= zEnd; ++k) {

        AABB cube{i, i + 1.0, j, j + 1.0, k, k + 1};
        vec3 normal;
        auto cubeId = m_chunkManager.getCubeId(i, j, k);
        if (!(cubeId == cube_data::AIR || cubeId == cube_data::WATER)) {
          vec3 vec;
          float time = AABB::collisionTime(start, cube, vec, movement);

          if (time < 1 && time >= 0)
            collisions.push_back(std::make_tuple(time, cubeId, vec));

        }
      }
    }
  }
}

bool Player::isInWater() {
//   Fix hardcoded shit...
  AABB box{m_location.x - 0.4, m_location.x + 0.4, m_location.y - 1.5,
      m_location.y + 0.1, m_location.z - 0.4, m_location.z + 0.4};

  int xStart = floor(box.m_xMin);
  int yStart = floor(box.m_yMin);
  int zStart = floor(box.m_zMin);

  int xEnd = floor(box.m_xMax);
  int yEnd = floor(box.m_yMax);
  int zEnd = floor(box.m_zMax);

  for (int i = xStart; i <= xEnd; i++) {
    for (int j = yStart; j <= yEnd; j++) {
      for (int k = zStart; k <= zEnd; k++) {
        auto cubeId = m_chunkManager.getCubeId(i, j, k);
        if (cubeId != WATER)
          return false;
      }
    }
  }
  return true;
}

AABB Player::createAABB() {
  return {m_location.x - m_width / 2, m_location.x + m_width / 2,
    m_location.y - m_cameraHeight, m_location.y + m_height - m_cameraHeight,
    m_location.z - m_width / 2, m_location.z + m_width / 2};
}

} /* namespace entity */
