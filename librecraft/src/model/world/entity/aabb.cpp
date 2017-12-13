#include "aabb.h"

#include <algorithm>
#include <iostream>

namespace entity {

AABB::AABB(double xMinimum,
           double xMaximum,
           double yMinimum,
           double yMaximum,
           double zMinimum,
           double zMaximum)
  : m_xMin{ xMinimum }
  , m_xMax{ xMaximum }
  , m_yMin{ yMinimum }
  , m_yMax{ yMaximum }
  , m_zMin{ zMinimum }
  , m_zMax{ zMaximum } {
}

void
AABB::setBounds(double xMinimum,
                double xMaximum,
                double yMinimum,
                double yMaximum,
                double zMinimum,
                double zMaximum) {

  m_xMin = xMinimum;
  m_xMax = xMaximum;
  m_yMin = yMinimum;
  m_yMax = yMaximum;
  m_zMin = zMinimum;
  m_zMax = zMaximum;
}

bool
AABB::intersects(const AABB& that) {
  return ((this->m_xMin >= that.m_xMin && this->m_xMin <= that.m_xMax) ||
          (this->m_xMax >= that.m_xMin && this->m_xMax <= that.m_xMax)) &&
         ((this->m_yMin >= that.m_yMin && this->m_yMin <= that.m_yMax) ||
          (this->m_yMax >= that.m_yMin && this->m_yMax <= that.m_yMax)) &&
         ((this->m_zMin >= that.m_zMin && this->m_zMin <= that.m_zMax) ||
          (this->m_zMax >= that.m_zMin && this->m_zMax <= that.m_zMax));
}

AABB
AABB::getSweptBroadPhaseBox(AABB& box, glm::vec3& velocity) {
  float xMin, xMax, yMin, yMax, zMin, zMax;

  if (velocity.x > 0.0f) {
    xMin = box.m_xMin;
    xMax = box.m_xMax + velocity.x;
  } else {
    xMin = box.m_xMin + velocity.x;
    xMax = box.m_xMax;
  }

  if (velocity.y > 0.0f) {
    yMin = box.m_yMin;
    yMax = box.m_yMax + velocity.y;
  } else {
    yMin = box.m_yMin + velocity.y;
    yMax = box.m_yMax;
  }

  if (velocity.z > 0.0f) {
    zMin = box.m_zMin;
    zMax = box.m_zMax + velocity.z;
  } else {
    zMin = box.m_zMin + velocity.z;
    zMax = box.m_zMax;
  }

  return AABB{ xMin, xMax, yMin, yMax, zMin, zMax };
}

float
AABB::collisionTime(AABB& box1,
                    AABB& box2,
                    glm::vec3& collisionNormal,
                    glm::vec3& velocity) {

  float xEntryDistance, yEntryDistance, zEntryDistance;
  float xExitDistance, yExitDistance, zExitDistance;

  if (velocity.x > 0.0f) {
    xEntryDistance = box2.m_xMin - box1.m_xMax;
    xExitDistance = box2.m_xMax - box1.m_xMin;
  } else {
    xEntryDistance = box2.m_xMax - box1.m_xMin;
    xExitDistance = box2.m_xMin - box1.m_xMax;
  }

  if (velocity.y > 0.0f) {
    yEntryDistance = box2.m_yMin - box1.m_yMax;
    yExitDistance = box2.m_yMax - box1.m_yMin;
  } else {
    yEntryDistance = box2.m_yMax - box1.m_yMin;
    yExitDistance = box2.m_yMin - box1.m_yMax;
  }

  if (velocity.z > 0.0f) {
    zEntryDistance = box2.m_zMin - box1.m_zMax;
    zExitDistance = box2.m_zMax - box1.m_zMin;
  } else {
    zEntryDistance = box2.m_zMax - box1.m_zMin;
    zExitDistance = box2.m_zMin - box1.m_zMax;
  }

  float xEntryTime, yEntryTime, zEntryTime;
  float xExitTime, yExitTime, zExitTime;

  if (velocity.x == 0) {
    xEntryTime = -std::numeric_limits<float>::infinity();
    xExitTime = std::numeric_limits<float>::infinity();
  } else {
    xEntryTime = xEntryDistance / velocity.x;
    xExitTime = xExitDistance / velocity.x;
  }

  if (velocity.y == 0) {
    yEntryTime = -std::numeric_limits<float>::infinity();
    yExitTime = std::numeric_limits<float>::infinity();
  } else {
    yEntryTime = yEntryDistance / velocity.y;
    yExitTime = yExitDistance / velocity.y;
  }

  if (velocity.z == 0) {
    zEntryTime = -std::numeric_limits<float>::infinity();
    zExitTime = std::numeric_limits<float>::infinity();
  } else {
    zEntryTime = zEntryDistance / velocity.z;
    zExitTime = zExitDistance / velocity.z;
  }

  // TODO Correct?
  if (xEntryTime > 1.0f)
    xEntryTime = -1000000;
  if (yEntryTime > 1.0f)
    yEntryTime = -1000000;
  if (zEntryTime > 1.0f)
    zEntryTime = -1000000;

  float entryTime = std::max(xEntryTime, std::max(yEntryTime, zEntryTime));
  float exitTime = std::min(xExitTime, std::min(yExitTime, zExitTime));

  // No collision
  if (entryTime > exitTime ||
      (xEntryTime < 0.0f && yEntryTime < 0.0f && zEntryTime < 0.0f))
    return 1.0f;

  // Collision!
  if (xEntryTime > yEntryTime && xEntryTime > zEntryTime) {
    collisionNormal = glm::vec3(1.0f, 0.0f, 0.0f);
  } else if (yEntryTime > xEntryTime && yEntryTime > zEntryTime) {
    collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f);
  } else {
    collisionNormal = glm::vec3(0.0f, 0.0f, 1.0f);
  }

  return entryTime;
}
}
